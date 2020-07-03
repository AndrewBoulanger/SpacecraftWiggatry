#pragma once
#ifndef _SPRITEMANAGER_H_
#define _SPRITEMANAGER_H_


#include "Sprite.h"
#include <vector>
#include <map>
#include "Engine.h"

enum SpriteType
{
	Regular, background, platform
};

class SpriteManager
{
public:

public:
	static void LoadLevel();
	static void Update();
	static void Render();

	static void PushSprite(Sprite* pSprite, SpriteType type);

	static void PopSprite();
	static void RemoveLevel(); //i dont know if we'll need this but im putting it here anyway
	static void Quit();
	static std::vector<Sprite*>& GetSprites();
	static std::vector<Sprite*>& GetPlatforms() { return s_platforms; }
	/*static std::vector<Sprite*>& GetEnemies() { return s_enemies; }
	static std::vector<Sprite*>& GetProjectiles() { return s_projectiles; }
	static std::vector<Sprite*>& GetPickups() { return s_pickups; }*/
	static void ScrollAll(float scroll);
	static float getOffset() { return offset; }
private:
	static std::vector<Sprite*> s_background;
	static std::vector<Sprite*> s_platforms;
	static std::vector<Sprite*> s_sprites;
	//static std::vector<Sprite*> s_enemies;
	//static std::vector<Sprite*> s_projectiles;
	//static std::vector<Sprite*> s_pickups;

	static float offset;
	SpriteManager() {};

	bool cleanBG, cleanPltf, cleanEn, cleanPr, CleanPups;
};


typedef SpriteManager SPMR;

#endif