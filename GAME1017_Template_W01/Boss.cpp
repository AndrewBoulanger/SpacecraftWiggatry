#include "Boss.h"

Boss::Boss(SDL_Rect s, SDL_FRect d, SDL_Renderer* r, SDL_Texture* t, int sstart, int smin, int smax, int nf, int hp, int dmg)
	:Enemy(s, d, r, t, sstart, smin, smax, nf, hp, dmg)
{
	enemysWig = (new Wig({ 0,0,50,50 }, { d.x,d.y,100,100 }, Engine::Instance().GetRenderer(), TEMA::GetTexture("wig")));

	hasWig = hasSpareWig = true;
	m_dir = -1;

	m_accelX = m_accelY = m_velX = m_velY = 0.0;
	m_maxVelX = 10.0;
	m_maxVelY = 10.0;
	m_grav = GRAV;
	m_drag = 0.88;

}


void Boss::Update()
{

	if (iCooldown > 0)
	{
		--iCooldown;
	}
}

void Boss::verticalMove(const int plrY)
{
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
		return new Wig({ 0,0,50,50 }, { m_dst.x,m_dst.y,100,100 }, Engine::Instance().GetRenderer(), TEMA::GetTexture("wig"));
	}

	else return nullptr;

}
