#include "VerticalEnemy.h"
#include "SpriteManager.h"
#include "CollisionManager.h"

VerticalEnemy::VerticalEnemy(SDL_Rect s, SDL_FRect d, SDL_Renderer* r, SDL_Texture* t, int sstart, int smin, int smax, int nf, int hp, int dmg)
	:Enemy(s, d, r, t, sstart, smin, smax, nf, hp, dmg)
{
	enemysWig = (new Wig({ 0,0,50,50 }, { d.x,d.y,40,40 }, Engine::Instance().GetRenderer(), TEMA::GetTexture("wig")));
	sight = new raycast();
	losMax = 50;
	hasWig = true;
	m_dir = -1;

	m_accelX = m_accelY = m_velX = m_velY = 0.0;
	m_maxVelX = 10.0;
	m_maxVelY = 10.0;
	m_grav = GRAV;
	m_drag = 0.88;
}

void VerticalEnemy::Update()
{
	lookTimer--;
	if (lookTimer <= 0)
	{
		LOSCheck();
	}
	sight->Update();

	m_velX += m_accelX;
	m_velX *= m_drag;
	m_velX = std::min(std::max(m_velX, -(m_maxVelX)), (m_maxVelX));

	m_dst.x += (int)m_velX;
	//Y 
	m_velY += m_accelY; // Adjust gravity to get slower jump.
	m_velY = std::min(std::max(m_velY, -(m_maxVelY)), (m_maxVelY));
	if (!COMA::PlayerCollision(&m_dst, 0, -m_velY*2))
		m_dst.y -= (int)m_velY; // To remove aliasing, I made cast it to an int too.
	else
	{
		StopY();
	}
	m_accelX = m_accelY = 0.0;

	if (iCooldown > 0)
	{
		--iCooldown;
	}
	if (hasWig)
	{
		enemysWig->SetPos({ (int)(m_dst.x + m_dst.w / 2) , (int)(m_dst.y + 20) });
	}

	if (state == idle)
	{
		if (COMA::CircleCircleCheck(getCenter(), SPMR::getPlayer()->getCenter(), 400) && ((m_dir == -1 && SPMR::getPlayer()->GetX() <= m_dst.x) || (m_dir == 1 && SPMR::getPlayer()->GetX() >= m_dst.x)))
		{
			std::cout << "player in enemy range\n";
			setState(seeking);
		}
	}
	else if (state == seeking)
	{
		int playerdir = m_dst.x - SPMR::getPlayer()->GetX();
		int playerHeight = m_dst.y - SPMR::getPlayer()->getCenter().y;
		if (playerdir >= 0)
			m_dir = -1;
		else if (playerdir < 0)
			m_dir = 1;
		verticalMove(playerHeight);

		if (playerdir > 600 || playerdir < -600)
			setState(idle);
	}
	else if (state == fleeing)
	{
		int playerdir = m_dst.x - SPMR::getPlayer()->GetX();
		if (playerdir > 0)
			m_dir = 1;
		else
			m_dir = -1;
		groundedMove2(m_dir);
		if (playerdir > 700 || playerdir < -700)
			readyToDelete = true;
	}

}
void VerticalEnemy::verticalMove(const int plrY)
{
	if (plrY < -75)
		m_accelY = -1;
	else if (plrY > 25)
		m_accelY = 1;
}

