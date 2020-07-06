#pragma once
#ifndef _SPRITEMANAGER_H_
#define _SPRITEMANAGER_H_


#include "Sprite.h"
#include <vector>
#include <map>
#include "Engine.h"
#include "Enemy.h"

enum SpriteType
{
	Regular, background
};

class SpriteManager
{
public:

public:
	static void LoadLevel();
	static void Update();
	static void Render();

	static void PushSprite(Sprite* pSprite, SpriteType type);
	static void PushSprite(Enemy* pSprite);
	static void PushSprite(Pickup* pSprite);

	static void PopSprite();
	static void RemoveLevel(); //i dont know if we'll need this but im putting it here anyway
	static void Quit();
	static std::vector<Sprite*>& GetSprites();
	static std::vector<Enemy*>& GetEnemies() { return s_enemies; }
	static std::vector<Sprite*>& GetProjectiles() { return s_projectiles; }
	static std::vector<Pickup*>& GetPickups() { return s_pickups; }
	static void ScrollAll(float scroll);
	static float getOffset() { return offset; }
private:
	static std::vector<Sprite*> s_background;
	static std::vector<Sprite*> s_sprites;
	static std::vector<Enemy*> s_enemies;
	static std::vector<Sprite*> s_projectiles;
	static std::vector<Pickup*> s_pickups;

	static float offset;
	SpriteManager() {};

	bool cleanBG, cleanPltf, cleanEn, cleanPr, CleanPups;
};


typedef SpriteManager SPMR;

#endif