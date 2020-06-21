#pragma once
#ifndef _SPRITEMANAGER_H_
#define _SPRITEMANAGER_H_


#include "Sprite.h"
#include <vector>

class SpriteManager
{
public:
	static std::vector<Sprite*> s_sprites;
public:
	static void Update();
	static void Render();
	static void PushSprite(Sprite* pSprite);
	static void PopState();
	static void Quit();
	static std::vector<Sprite*>& GetSprites();
private:
	SpriteManager() {}
};


typedef SpriteManager SPMR;

#endif