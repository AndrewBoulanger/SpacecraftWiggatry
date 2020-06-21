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
	pPos = { playerdst->x, playerdst->y };
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
	SDL_Rect temp;
	SDL_Rect Hookdst;
	SDL_Rect Platformdst;
	SDL_FRect** PlatformArray = ((GameState*)(STMA::GetStates().back()))->getPlatform();

	Hookdst.x = m_dst.x;
	Hookdst.y = m_dst.y;
	Hookdst.w = m_dst.w;
	Hookdst.h = m_dst.h;

	for (int i = 0; i < NUMPLATFORMS; ++i)
	{
		Platformdst.x = PlatformArray[i]->x;
		Platformdst.y = PlatformArray[i]->y;
		Platformdst.w = PlatformArray[i]->w;
		Platformdst.h = PlatformArray[i]->h;

		if (SDL_IntersectRect(&Hookdst, &Platformdst, &temp))
		{
			Hookdst.y = Hookdst.y + temp.h;
			//m_dst.y = Hookdst.y;
			hookFixed = true;
		}
	}
}


float Hookshot::MyLerp(float a, float b, float t)
{
	return a + t * (b - a);
}

void Hookshot::Update()
{
	if (hookFixed == false)
	{
		move();
	}
	
	Collision();

	if (hookFixed == true)
	{
		if (lerpCo <= 1.0f)
		{
			playerdst->x = MyLerp(playerdst->x, m_dst.x  + (m_dst.w * 0.5) - (playerdst->w * 0.5), lerpCo);
			playerdst->y = MyLerp(playerdst->y, m_dst.y, lerpCo);
			lerpCo += 0.01f;
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