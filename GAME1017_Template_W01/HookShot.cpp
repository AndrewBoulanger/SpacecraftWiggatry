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
#include "SpriteManager.h"


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
	m_dst.x += velX * 0.2;
	m_dst.y += velY * 0.2;
}

void Hookshot::Collision()
{

	if (COMA::SmallTileCollision(m_dst, velX*.2 , velY*.2))
		{
			hookFixed = true;
		}
	for (int i = 0; i < SPMR::GetEnemies().size(); i++)
	{
		if (enemyHit == false && COMA::CircleAABBCheck(getCenter(), m_dst.w * .5, *SPMR::GetEnemies()[i]->GetDstP()))
		{
			enemyHit = true;
			if (SPMR::GetEnemies()[i]->getHealth() <= 0 && SPMR::GetEnemies()[i]->getHasWig())
			{
				stolenWig = SPMR::GetEnemies()[i]->removeWig();
				SPMR::PushSprite(stolenWig);
			}
		}
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

	if (hookFixed == true && !enemyHit )
	{
		int lerpX = MyLerp(playerdst->x, m_dst.x + (m_dst.w * 0.5) - (playerdst->w * 0.5), lerpCo);
		int lerpY = MyLerp(playerdst->y, m_dst.y - (m_dst.h *0.5), lerpCo);
		if ((lerpCo < .3) && !(MAMA::Distance(m_dst.x,playerdst->x, m_dst.y-m_dst.h*.5, playerdst->y) < 34) )
		{
			playerdst->x = lerpX;
			playerdst->y = lerpY;
			lerpCo += 0.01f;
			grav = 0;
			
		}
		else
		{
			deactivateHookshot();
			grav = GRAV;
		}

	}

	if (enemyHit)
	{
		if (lerpCo <= .3f)
		{
			m_dst.x = MyLerp(m_dst.x, playerdst->x + (playerdst->w*.5) , lerpCo);
			m_dst.y = MyLerp(m_dst.y, playerdst->y + (playerdst->h*.5), lerpCo);
			lerpCo += 0.01f;
			if (stolenWig != nullptr)
			{
				stolenWig->SetPos({(int) m_dst.x,(int) m_dst.y });
			}
		}
		else
		{
			deactivateHookshot();
			stolenWig = nullptr;
		}
	}

	if (m_dst.x <= 0 || m_dst.x >= 1000 || m_dst.y <= 0 || m_dst.y >= 740)
	{
		deactivateHookshot();
	}

	//if (active)
		this->Render();
}

void Hookshot::Render()
{
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

void Hookshot::deactivateHookshot()
{
	m_dst.x = playerdst->x + (m_dst.w * 0.5) - (playerdst->w * 0.5);
	m_dst.y = playerdst->y;
	active = false;
	lerpCo = 0;
	hookFixed = false;
	enemyHit = false;
}
