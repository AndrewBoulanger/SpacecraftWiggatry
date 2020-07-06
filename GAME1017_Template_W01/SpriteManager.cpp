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
	if (!s_sprites.empty()) // empty() and back() are methods of the vector type.
	{
		for(int i = 0; i < s_sprites.size(); i++)
		{
			if (s_sprites[i]->readyToDelete)
			{
				delete s_sprites[i];
				s_sprites[i] = nullptr;
			//	s_sprites.erase(s_sprites[i]);
			}
		}
	}
}

void SpriteManager::Render()
{
	for (int i = 0; i < s_sprites.size(); i++)
		s_sprites[i]->Render();
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
		s_background[i]->GetDstP()->x -= scroll;
	}

	
	offset += scroll;
}

std::vector<Sprite*> SpriteManager::s_sprites;
std::vector<Sprite*> SpriteManager::s_background;
std::vector<Enemy*> SpriteManager::s_enemies;
std::vector<Pickup*> SpriteManager::s_pickups;
std::vector<Sprite*> SpriteManager::s_projectiles;
float SpriteManager::offset;