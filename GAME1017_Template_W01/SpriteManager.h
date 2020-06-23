#pragma once
#ifndef _SPRITEMANAGER_H_
#define _SPRITEMANAGER_H_


#include "Sprite.h"
#include <vector>
#include <map>
#include "Engine.h"

class SpriteManager
{
public:
	static std::vector<Sprite*> s_sprites;
public:
	static void LoadLevel();
	static void Update();
	static void Render();
	static void PushSprite(Sprite* pSprite);
	static void PopSprite();
	static void RemoveLevel(); //i dont know if we'll need this but im putting it here anyway
	static void Quit();
	static std::vector<Sprite*>& GetSprites();
	std::array < std::array<Tile*, COLS>, ROWS>& GetLevel() { return m_level; }
private:
	SpriteManager() {};
	std::array<std::array<Tile*, COLS>, ROWS> m_level;
	std::map<char, Tile*> m_tiles;
};


typedef SpriteManager SPMR;

#endif