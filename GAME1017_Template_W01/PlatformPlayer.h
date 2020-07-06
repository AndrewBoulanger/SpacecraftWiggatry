#pragma once
#ifndef _PLATFORMPLAYER_H_
#define _PLATFORMPLAYER_H_

//gravity was moved to character class so the enemies can use it too
#define JUMPFORCE 35.0

#include "Sprite.h"
#include "Character.h"
#include "StunGun.h"
#include <vector>
#include <array>

class Hookshot;
class Wig;
class StunGun;

class PlatformPlayer : public Character
{
public:
	PlatformPlayer(SDL_Rect s, SDL_FRect d, SDL_Renderer* r, SDL_Texture* t, int sstart = 0, int smin = 0, int smax = 0, int nf = 0);
	~PlatformPlayer();
	void Update();
	void Stop();

	double GetThurst();
	bool IsGrounded();
	void SetGrounded(bool g);

	void SetGrav(double grav) { m_grav = grav; }
	
	void SetX(float y);
	void SetY(float y);
	double GetX();
	virtual void Render();
	void KnockLeft(double vel);
	void takeDamage(int dmg) override;
	int getWigs()
	{
		return m_wigCount;
	};
	void setWigs(int wigs) 
	{
		m_wigCount = wigs;
	}
	void add1Wig()
	{
		++m_wigCount;
	}
	int getParts()
	{
		return m_ShipParts;
	};
	void setParts(int parts)
	{
		m_ShipParts = parts;
	}
	void add1ShipPart()
	{
		++m_ShipParts;
	}

	void slap();
	void createStunGunBullet();
	void StunGunCollision();
	void BulletBoundCheck();

	Hookshot* getHookShot() { return m_hookShot; }


private:
	bool m_grounded;

	bool m_facingRight = true;
	bool m_facingUp = false;

	int m_wigCount = 0;
	int m_ShipParts = 0;
	float m_distance = 0;
	Hookshot* m_hookShot;

	std::vector<StunGun*> m_vPBullets;

	
};

#endif