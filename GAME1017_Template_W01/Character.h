#pragma once
#include "Sprite.h"
#include "Engine.h"

#define GRAV 2.0f

class Character : public AnimatedSprite
{
protected:

	int health;
	int baseDamage = 1;
	int iFrames = 60;
	int iCooldown;

	double m_accelX,
		m_accelY,
		m_velX,
		m_maxVelX,
		m_velY,
		m_maxVelY,
		m_drag,
		m_thrust,
		m_grav;

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
	void setHealth(int hp)
	{
		health = hp;
	};
	void setBaseDamage(int damage)
	{
		baseDamage = damage;
	}
	void setICoolDown(int frames)
	{
		iCooldown = frames;
	}
	void incrHealth() { health = ((health +1) >5) ? 5: ++health; }

	virtual void takeDamage(int dmg);


	void StopX();
	void StopY();
	void SetAccelX(double a);
	void SetAccelY(double a);
	double GetAccelX();
	double GetAccelY();
	double GetVelX();
	double GetVelY();
	void SetVelY(double a);
	void SetVelX(double a);
	void SetGrav(double grav) { m_grav = grav; }

};

