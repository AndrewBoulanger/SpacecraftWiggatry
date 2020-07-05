#include "StunGun.h"
#include <algorithm> // For min/max.
#include <cmath>	 // For cos/sin.
#include "Sprite.h"
#include "Engine.h"
using namespace std;

StunGun::StunGun(SDL_Rect src, SDL_FRect dst, SDL_Renderer* r, SDL_Texture* t) :
	Sprite(src, dst, r, t) {}

StunGun::~StunGun()
{

}

void StunGun::update()
{
	//m_dst.x += m_speed;
	m_dst.x += velX;
	m_dst.y += velY;
}

void StunGun::render()
{
	SDL_RenderCopyF(m_pRend, m_pText, &m_src, &m_dst);
}