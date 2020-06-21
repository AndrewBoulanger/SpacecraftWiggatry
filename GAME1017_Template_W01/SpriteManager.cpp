#include "SpriteManager.h"

void SpriteManager::Update()
{
	if (!s_sprites.empty()) // empty() and back() are methods of the vector type.
	{
		for(int i = 0; i < s_sprites.size(); i++)
		{
			s_sprites[i]->Update();
		}
	}
}

void SpriteManager::Render()
{
	if (!s_sprites.empty())
		s_sprites.back()->Render();
}

void SpriteManager::PushSprite(Sprite* pSprite)
{
	s_sprites.push_back(pSprite); // push_back() is a method of the vector type.
	
}

void SpriteManager::PopState()
{
	if (!s_sprites.empty())
	{
		delete s_sprites.back();
		s_sprites.back() = nullptr;
		s_sprites.pop_back();
	}
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

std::vector<Sprite*> SpriteManager::s_sprites;