#include "Enemy.h"
#include "SpriteManager.h"
#include "CollisionManager.h"

Enemy::Enemy(SDL_Rect s, SDL_FRect d, SDL_Renderer* r, SDL_Texture* t, int sstart, int smin, int smax, int nf, int hp, int dmg)
	:Character(s, d, r, t, sstart, smin, smax, nf, hp, dmg) 
{
	enemysWig = (new Wig({ 0,0,50,50 }, { d.x,d.y,40,40 }, Engine::Instance().GetRenderer(), TEMA::GetTexture("wig")));
	
	hasWig = true;
	m_dir = -1;

	m_accelX = m_accelY = m_velX = m_velY = 0.0;
	m_maxVelX = 10.0;
	m_grav = GRAV;
	m_drag = 0.88;
}

void Enemy::setState(enemyState nState)
{
	state = nState;
	if (nState == idle)
		m_frameMax = 1; //only show one frame
	else if (nState == seeking)
		m_frameMax = 3;
	//else if(nState == fleeing) need to make sure fleeing sprite is in the same texture file as the normal sprite
}


void Enemy::Update()
{
	//X
	m_velX += m_accelX;
	m_velX *= m_drag;
	m_velX = std::min(std::max(m_velX, -(m_maxVelX)), (m_maxVelX));
	if (!COMA::PlayerCollision(&m_dst, m_velX, 0, SPMR::getOffset()))
		m_dst.x += (int)m_velX;
	else
		StopX();
	//Y
	m_velY += m_accelY + m_grav; // Adjust gravity to get slower jump.
	m_velY = std::min(std::max(m_velY, -(m_maxVelY)), (m_grav * 5));
	if (!COMA::PlayerCollision(&m_dst, 0, m_velY, SPMR::getOffset()))
		m_dst.y += (int)m_velY; // To remove aliasing, I made cast it to an int too.
	else
		StopY();

	if (hasWig)
	{
		enemysWig->SetPos({ (int)(m_dst.x + m_dst.w/2) , (int)(m_dst.y +20)});
	}

	if (state == idle)
	{
		if (COMA::CircleCircleCheck(getCenter(), SPMR::getPlayer()->getCenter(), 400))
		{
			std::cout << "player in enemy range\n";
			setState(seeking);
		}
	}

}

void Enemy::Render()
{
	if (iCooldown % 10 < 5)
	{
		if (m_dir == -1)
			Sprite::Render();
		else
			Sprite::RenderFlipped();
		if (hasWig)
		{
			if (m_dir == -1)
				enemysWig->RenderFlipped();
			else
				enemysWig->Render();
		}
	}
}