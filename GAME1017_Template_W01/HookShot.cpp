#include "HookShot.h"
#include "EventManager.h"
#include "MathManager.h"
#include "States.h"
#include "StateManager.h"
#include "TextureManager.h"
#include "math.h"
#include <iostream>
#include "CollisionManager.h"
#include "EventManager.h"


Hookshot::Hookshot(SDL_Rect src, SDL_FRect dst, SDL_Renderer* r, SDL_Texture* t)
	: Sprite(src, dst, r, t)
{
	line.x = 0; //player
	line.y = 0;
	line.w = 0; //hook
	line.h = 0;
}

Hookshot::~Hookshot()
{
	playerdst = nullptr;
}

void Hookshot::calHookAngle(SDL_FRect* playerPos)
{
	playerdst = playerPos;
	
	SDL_Point mPos = EVMA::GetMousePos();

	shotAngle = -MAMA::Rad2Deg(MAMA::AngleBetweenPoints(mPos.y - (playerdst->y + playerdst->h * .5), mPos.x - (playerdst->x + playerdst->w * .5)));

		velX = cos(MathManager::Deg2Rad(shotAngle)) * speed;
			velY = -sin(MathManager::Deg2Rad(shotAngle)) * speed;
			int a = 10;
}

void Hookshot::move()
{
	m_dst.x += velX * 0.1;
	m_dst.y += velY * 0.1;
}

void Hookshot::Collision()
{

	if (COMA::SmallTileCollision(m_dst, velX *.1, velY*.1))
		{
			hookFixed = true;
		}
	
}


float Hookshot::MyLerp(float a, float b, float t)
{
	return a + t * (b - a);
}

void Hookshot::Update(double& grav)
{
	if (hookFixed == false)
	{
		move();
		Collision();
	}

	if (hookFixed == true)
	{
		if (lerpCo <= 1.0f)
		{
			playerdst->x = MyLerp(playerdst->x, m_dst.x  + (m_dst.w * 0.5) - (playerdst->w * 0.5), lerpCo);
			playerdst->y = MyLerp(playerdst->y, m_dst.y, lerpCo);
			lerpCo += 0.01f;
			grav = 0;
		}
		else
		{
			playerdst->x = m_dst.x + (m_dst.w * 0.5) - (playerdst->w * 0.5);
			playerdst->y = m_dst.y;
		}
	}
}

void Hookshot::Render()
{
	SDL_Rect m_srcline = { 0, 0, 18, 17 };
	SDL_FRect m_dstline = { line.x, line.y, line.w, line.h };
	//SDL_RenderCopyExF(m_pRend, TEMA::GetTexture("line(temp)"), &m_srcline, &m_dstline, -shotAngle, nullptr, SDL_FLIP_NONE);
	//SDL_RenderCopyF(m_pRend, TEMA::GetTexture("line(temp)"), &m_srcline, &m_dstline);
	SDL_RenderCopyF(m_pRend, m_pText, &m_src, &m_dst);
}

void Hookshot::sethookFixed(bool b)
{
	hookFixed = b;
}

void Hookshot::setlerpCo(float lc)
{
	lerpCo = lc;
}