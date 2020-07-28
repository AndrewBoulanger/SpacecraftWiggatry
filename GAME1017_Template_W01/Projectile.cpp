#include "Projectile.h"
#include "Sprite.h"
#include "Engine.h"
#include "MathManager.h"


Projectile::Projectile(SDL_Rect src, SDL_FRect dst, SDL_Renderer* r, SDL_Texture* t) :
	Sprite(src, dst, r, t) {};

Projectile::Projectile(bool fromPlayer, SDL_FPoint origin, double angle, float size, SDL_Texture* t)
{
	m_pRend = Engine::Instance().GetRenderer();
	m_src = { 0,0,36,36 };
	m_dst = { origin.x, origin.y, 32*size, 32* size };
	m_pText = t;
	owner = (fromPlayer) ? PLAYER_BULLET : ENEMY_BULLET;

	velX = cos(MathManager::Deg2Rad(angle));
	velY = -sin(MathManager::Deg2Rad(angle));
	m_size = size;
}

Projectile::~Projectile() 
{
};

void Projectile::update()
{
	m_dst.x += velX * m_speed / m_size;
	m_dst.y += velY * m_speed / m_size;

	if (m_dst.x <0 - m_dst.w || m_dst.x > WIDTH + m_dst.w ||
		m_dst.y <0 - m_dst.h || m_dst.y > HEIGHT + m_dst.h)
	{
		readyToDelete = true;;
	}
}

void Projectile::render()
{
	SDL_RenderCopyF(m_pRend, m_pText, &m_src, &m_dst);
}