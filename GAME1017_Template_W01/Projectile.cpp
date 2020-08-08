#include "Projectile.h"
#include "Sprite.h"
#include "Engine.h"
#include "MathManager.h"


Projectile::Projectile(SDL_Rect src, SDL_FRect dst, SDL_Renderer* r, SDL_Texture* t) :
	AnimatedSprite(src, dst, r, t, 0, 0, 4,4) {};

Projectile::Projectile(bool fromPlayer, SDL_FPoint origin, double angle, float size, SDL_Texture* t)
{
	m_pRend = Engine::Instance().GetRenderer();
	m_src = { 95, 0,25,28 };
	m_sprite = m_frame = m_spriteMin = 0;
	m_spriteMax = 3;
	m_frameMax = 4;
	m_dst = { origin.x, origin.y, 32*size, 32* size };
	m_pText = t;
	owner = (fromPlayer) ? PLAYER_BULLET : ENEMY_BULLET;
	m_angle = angle;

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
	
	Animate();

	if (m_dst.x <0 - m_dst.w || m_dst.x > WIDTH + m_dst.w ||
		m_dst.y <0 - m_dst.h || m_dst.y > HEIGHT + m_dst.h)
	{
		readyToDelete = true;;
	}
}

void Projectile::Render()
{
//	SDL_RenderCopyF(m_pRend, m_pText, &m_src, &m_dst);
	SDL_RenderCopyExF(m_pRend, m_pText, &m_src, &m_dst, -m_angle -90, 0, SDL_FLIP_NONE);
}