#pragma once
#include "Sprite.h"
class Character : public Sprite
{
protected:

	int health;
	int speed;
	int baseDamage;

public:
	Character(SDL_Rect s, SDL_FRect d, SDL_Renderer* r, SDL_Texture* t);

	//getters
	int getHealth() const
	{
		return health;
	};
	int getSpeed() const
	{
		return speed;
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
	void setSpeed(int speed)
	{
		this->speed = speed;
	};
	void setBaseDamage(int damage)
	{
		baseDamage = damage;
	}


};

