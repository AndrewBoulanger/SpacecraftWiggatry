#pragma once
#include "Sprite.h"
#include "Engine.h"

class Character : public AnimatedSprite
{
protected:

	int health;
	int baseDamage;
	int iFrames = 60;
	int iCooldown;

public:
	Character(SDL_Rect s, SDL_FRect d, SDL_Renderer* r, SDL_Texture* t, int sstart = 0, int smin = 0, int smax = 0, int nf = 0, int hp = 50, int dmg =10);

	//getters
	int getHealth() const
	{
		return health;
	};
	int getBaseDamage() const
	{
		return baseDamage;
	};
	int getICoolDown()
	{
		return iCooldown;
	}

	//setters
	void setHealth(int health)
	{
		this->health = health;
	};
	void setBaseDamage(int damage)
	{
		baseDamage = damage;
	}
	void setICoolDown(int frames)
	{
		iCooldown = frames;
	}

	void Render();

	virtual void takeDamage(int dmg);

};

