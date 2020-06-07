#pragma once
#include "Sprite.h"
#include "Engine.h"

class Character : public Sprite
{
protected:

	int health;
	int baseDamage;

public:
	Character(SDL_Rect s, SDL_FRect d, SDL_Renderer* r, SDL_Texture* t);

	//getters
	int getHealth() const
	{
		return health;
	};
	int getBaseDamage() const
	{
		return baseDamage;
	};

	//setters
	void setHealth(int health)
	{
		this->health = health;
	};
	void setBaseDamage(int damage)
	{
		baseDamage = damage;
	}

	void Render();

	virtual void takeDamage(int dmg);

};

