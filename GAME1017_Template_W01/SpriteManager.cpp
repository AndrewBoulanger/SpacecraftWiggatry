#include "SpriteManager.h"
#include <fstream>
#include "Tile.h"
#include "TextureManager.h"
#include "SoundManager.h"
#include "Engine.h"
#include "CollisionManager.h"
#include "Utilities.h"
#include "DebugManager.h"


void SpriteManager::LoadLevel()
{
}

void SpriteManager::Update()
{
	for (int i = 0; i < s_sprites.size(); i++)
	{
		DEMA::QueueRect(MAMA::ConvertFRect2Rect(*s_sprites[i]->GetDstP()), { 0,255,0,0 });
		if (s_sprites[i]->readyToDelete)
		{
			delete s_sprites[i];
			s_sprites[i] = nullptr;
			cleanSpr = true;
		}
	}
	if(cleanSpr)
		CleanVector(s_sprites, cleanSpr);
	for (int i = 0; i < s_enemies.size(); i++)
	{
		DEMA::QueueRect(MAMA::ConvertFRect2Rect(*s_enemies[i]->GetDstP()), { 0,255,0,0 });
		if (s_enemies[i]->readyToDelete)
		{
			delete s_enemies[i];
			s_enemies[i] = nullptr;
			cleanEn = true;
		}
		if(s_enemies[i] != nullptr)
			s_enemies[i]->Update();
	}
	if(cleanEn)
		CleanVector(s_enemies, cleanEn);
	for (int i = 0; i < s_pickups.size(); i++)
	{
		DEMA::QueueRect(MAMA::ConvertFRect2Rect(*s_pickups[i]->GetDstP()), { 0,255,0,0 });
		if (s_pickups[i] != nullptr)
			s_pickups[i]->Update();
	}

	Collision();
}

void SpriteManager::Collision()
{
	for (int i = 0; i < SPMR::GetPickups().size(); i++)
	{
		if (COMA::CircleCircleCheck(s_player->getCenter(), s_pickups[i]->getCenter(), 40, 50))
		{
			switch (s_pickups[i]->getType())
			{
			case WIG:
				s_player->add1Wig();
				break;
			case SHIP_PART:
				SOMA::PlaySound("yahoo");
				s_player->add1ShipPart();
				break;
			case ENERGY:
				SOMA::PlaySound("yahoo");
				s_player->raiseEnergy();
				break;
			case HEALTH:
				SOMA::PlaySound("yahoo");
				s_player->incrHealth();
			default:
				break;
			}
			delete s_pickups[i];
			s_pickups[i] = nullptr;
			std::cout << "collected\n";
			CleanVector(s_pickups);
		}
	}
	s_sprites.shrink_to_fit();
}

void SpriteManager::Render()
{
	for (int i = 0; i < s_background.size(); i++)
		s_background[i]->Render();
	s_player->Render();
	for (int i = 0; i < s_sprites.size(); i++)
		s_sprites[i]->Render();
	for (int i = 0; i < s_enemies.size(); i++)
		s_enemies[i]->Render();
	for (int i = 0; i < s_pickups.size(); i++)
		s_pickups[i]->Render();

	
	DEMA::FlushLines();
}

void SpriteManager::PushSprite(Sprite* pSprite, SpriteType type)
{
	if (pSprite != nullptr)
	{
		if (type == Regular)
			s_sprites.push_back(pSprite); // push_back() is a method of the vector type.

		if (type == background)
			s_background.push_back(pSprite);
	}
}
void SpriteManager::PushSprite(Enemy* pSprite)
{
	if (pSprite != nullptr)
	{
		s_enemies.push_back(pSprite);
	}
}
void SpriteManager::PushSprite(Pickup* pSprite)
{
	if (pSprite != nullptr)
	{
		s_pickups.push_back(pSprite);
	}
}

void SpriteManager::PopSprite()
{
	if (!s_sprites.empty())
	{
		delete s_sprites.back();
		s_sprites.back() = nullptr;
		s_sprites.pop_back();
	}
}

void SpriteManager::RemoveLevel()
{
	while(!s_background.empty())
	{
		delete [] s_background.back();
		s_background.back() = nullptr;
		s_background.pop_back();
	}
	while (!s_pickups.empty())
	{
		delete[] s_pickups.back();
		s_pickups.back() = nullptr;
		s_pickups.pop_back();
	}
	while (!s_enemies.empty())
	{
		delete[] s_enemies.back();
		s_enemies.back() = nullptr;
		s_enemies.pop_back();
	}

	offset = 0;
}

void SpriteManager::Quit()
{
	while (!s_sprites.empty())	
	{							
		delete s_sprites.back();
		s_sprites.back() = nullptr;
		s_sprites.pop_back();
	}
}

std::vector<Sprite*>& SpriteManager::GetSprites() { return s_sprites; }

void SpriteManager::ScrollAll(float scroll)
{
	for (int i = 0; i < s_sprites.size(); i++)
	{
		s_sprites[i]->GetDstP()->x -= scroll;
	}
	for (int i = 0; i < s_background.size(); i++)
	{
		s_background[i]->GetDstP()->x -= scroll;
	}
	for (int i = 0; i < s_enemies.size(); i++)
	{
		s_enemies[i]->GetDstP()->x -= scroll;
	}
	for (int i = 0; i < s_pickups.size(); i++)
	{
		s_pickups[i]->GetDstP()->x -= scroll;
	}
	s_player->GetDstP()->x -= scroll;
	
	offset += scroll;
}

//the more I add to this class, which is meant to hold all sprites, the more I realize I probably could have just made it a singleton
	//at least i dont have to call SPMR::Instance() before every function call
std::vector<Sprite*> SpriteManager::s_sprites;
std::vector<Sprite*> SpriteManager::s_background;
std::vector<Enemy*> SpriteManager::s_enemies;
std::vector<Pickup*> SpriteManager::s_pickups;
std::vector<Sprite*> SpriteManager::s_projectiles;
PlatformPlayer* SpriteManager::s_player;
float SpriteManager::offset;
bool SpriteManager::cleanSpr;
bool SpriteManager::cleanEn;