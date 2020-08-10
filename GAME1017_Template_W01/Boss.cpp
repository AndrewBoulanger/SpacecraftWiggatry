#include "Boss.h"
#include "SoundManager.h"
#include "SpriteManager.h"
#include "StateManager.h"
#include "EventManager.h"
#include "States.h"
#include "Projectile.h";

Boss::Boss(SDL_Rect s, SDL_FRect d, SDL_Renderer* r, SDL_Texture* t, int sstart, int smin, int smax, int nf, int hp, int dmg)
	:Enemy(s, d, r, t, sstart, smin, smax, nf, hp, dmg)
{
	enemysWig = (new Wig({ 0,0,50,50 }, { d.x,d.y,100,100 }, Engine::Instance().GetRenderer(), TEMA::GetTexture("wig")));

	hasWig = hasSpareWig = true;
	m_dir = -1;

	m_accelX = m_accelY = m_velX = m_velY = 0.0;
	m_maxVelX = 15.0;
	m_maxVelY = 20.0;
	m_grav = GRAV;
	m_drag = 0.88;

	m_frameMax = 0;
	m_spriteMax = 2;
	m_frame = m_sprite = m_spriteMin = 0;

	stateTimer = actionTimer =  120;

	health = 10;

	for (int i = 0; i < 5; i++)
		hpEnemyUI[i] = new Sprite({ 0,0, 256,256 }, { (float)(829 + 35 * i),0, 35,35 }, Engine::Instance().GetRenderer(), TEMA::GetTexture("heart1"));
	for (int i = 5; i < 10; i++)
		hpEnemyUI[i] = new Sprite({ 0,0, 256,256 }, { (float)(829 + 35 * (i - 5)),0, 35,35 }, Engine::Instance().GetRenderer(), TEMA::GetTexture("heart2"));
	bossMusicPlaying = false;
}


void Boss::Update()
{
	//move x
	m_velX += m_accelX;
	m_velX *= m_drag;
	m_velX = std::min(std::max(m_velX, -(m_maxVelX)), (m_maxVelX));
	m_dst.x += (int)m_velX;

	m_velY += m_accelY + m_grav; // Adjust gravity to get slower jump.
	m_velY = std::min(std::max(m_velY, -(m_maxVelY)), (m_grav * 5));
	if(m_dst.y+ m_dst.h + m_velY < HEIGHT-30 )
		m_dst.y += (int)m_velY; // To remove aliasing, I made cast it to an int too.
	else
		StopY();
	m_accelX = m_accelY = 0.0;


	if (state == idle)
	{
		if (m_dst.x <= WIDTH)
		{
			if (!bossMusicPlaying)
			{
				SOMA::PlayMusic("WreckingBall", -1);
				bossMusicPlaying = true;
			}
			state = seeking; //not using set state becuase we dont need to change the frames between animations
		}
	}
	else if (state == seeking)
	{
		stateTimer--;
		actionTimer--;

		if (stateTimer <= 0)
		{
			stateTimer = 240;
			actionTimer = 120;
			isShooting = (rand() % 10 > 5) ? true : false;
		}
		if (actionTimer <= 0)
		{
			actionTimer = 59;
			if (isShooting)
				Shoot();
			else
				Move();
		}
	}
	else
	{
		Flee();
		Animate();
	}

	if (iCooldown > 0)
	{
		--iCooldown;
	}
}

void Boss::Move()
{
	std::cout << "moved\n";
	m_accelX = -20;
	m_accelY = -20;
	Animate();
}

void Boss::Shoot()
{
	SDL_FPoint plr;
	plr.x = SPMR::getPlayer()->getCenter().x - getCenter().x;
	plr.y = SPMR::getPlayer()->getCenter().y - (m_dst.y + m_dst.h * .2f);

	std::cout << "shot\n";
	SPMR::PushSprite(new Projectile(false, { getCenter().x, m_dst.y + m_dst.h * .2f },
		-MAMA::Rad2Deg(MAMA::AngleBetweenPoints(plr.y, plr.x)) , 2, TEMA::GetTexture("laser")));
}

Wig* Boss::removeWig()
{
	if (hasSpareWig)
	{
		hasSpareWig = false;
		setHealth(5);
		m_src.x += m_src.w;
		return new Wig({ 0,0,50,50 }, { m_dst.x,m_dst.y,100,100 }, Engine::Instance().GetRenderer(), TEMA::GetTexture("wig"));
	}
	else if (hasWig && health <= 0)
	{
		hasWig = false;
		m_src.x += m_src.w;
		setState(fleeing);
		return new Wig({ 0,0,50,50 }, { m_dst.x,m_dst.y,100,100 }, Engine::Instance().GetRenderer(), TEMA::GetTexture("wig"));
	}

	else return nullptr;

}


void Boss::Render()
{
	if (iCooldown % 10 < 5)
	{
		if (m_dir == -1)
			Sprite::Render();
		else
			Sprite::RenderFlipped();
	}
	if(state != idle)
	for (int i = 0; i < health; i++)
		hpEnemyUI[i]->Render();

}