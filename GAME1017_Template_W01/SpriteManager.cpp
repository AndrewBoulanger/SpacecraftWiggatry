#include "SpriteManager.h"
#include <fstream>
#include "Tile.h"
#include "TextureManager.h"
#include "Engine.h"
#include "CollisionManager.h"
#include "Utilities.h"


void SpriteManager::LoadLevel()
{
}

void SpriteManager::Update()
{
	for (int i = 0; i < s_pickups.size(); i++)
	{
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
				s_player->add1ShipPart();
				break;
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
	for (int i = 0; i < s_sprites.size(); i++)
		s_sprites[i]->Render();
	for (int i = 0; i < s_enemies.size(); i++)
		s_enemies[i]->Render();
	for (int i = 0; i < s_pickups.size(); i++)
		s_pickups[i]->Render();
}

void SpriteManager::PushSprite(Sprite* pSprite, SpriteType type)
{
	if(type == Regular)
		s_sprites.push_back(pSprite); // push_back() is a method of the vector type.
		
	if(type == background)
		s_background.push_back(pSprite);
	
}
void SpriteManager::PushSprite(Enemy* pSprite)
{
	s_enemies.push_back(pSprite);
}
void SpriteManager::PushSprite(Pickup* pSprite)
{
	s_pickups.push_back(pSprite);
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
std::vector<Sprite*> SpriteManager::s_sprites;
std::vector<Sprite*> SpriteManager::s_background;
std::vector<Enemy*> SpriteManager::s_enemies;
std::vector<Pickup*> SpriteManager::s_pickups;
std::vector<Sprite*> SpriteManager::s_projectiles;
PlatformPlayer* SpriteManager::s_player;
float SpriteManager::offset;