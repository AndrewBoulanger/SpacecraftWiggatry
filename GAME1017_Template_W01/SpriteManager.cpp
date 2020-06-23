#include "SpriteManager.h"
#include <fstream>
#include "Tile.h"
#include "TextureManager.h"
#include "Engine.h"
#include "CollisionManager.h"


void SpriteManager::LoadLevel()
{
	//std::ifstream inFile("Dat/Tiledata.txt");
	//if (inFile.is_open())
	//{ // Create map of Tile prototypes.
	//	char key;
	//	int x, y;
	//	bool o, h;
	//	while (!inFile.eof())
	//	{
	//		inFile >> key >> x >> y >> o >> h;
	//		m_tiles.emplace(key, new Tile({ x * 32, y * 32, 32, 32 }, { 0,0,32,32 }, Engine::Instance().GetRenderer(), TEMA::GetTexture("tiles"), o, h));
	//	}
	//}
	//inFile.close();
	//inFile.open("Dat/Level1.txt");
	//if (inFile.is_open())
	//{ // Build the level from Tile prototypes.
	//	char key;
	//	for (int row = 0; row < ROWS; row++)
	//	{
	//		for (int col = 0; col < COLS; col++)
	//		{
	//			inFile >> key;
	//			m_level[row][col] = m_tiles[key]->Clone(); // Prototype design pattern used.
	//			m_level[row][col]->GetDstP()->x = (float)(32 * col);
	//			m_level[row][col]->GetDstP()->y = (float)(32 * row);
	//		}
	//	}
	//}
	//inFile.close();
}

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

void SpriteManager::PopSprite()
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