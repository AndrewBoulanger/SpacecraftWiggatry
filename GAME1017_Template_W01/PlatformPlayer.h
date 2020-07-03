#pragma once
#ifndef _PLATFORMPLAYER_H_
#define _PLATFORMPLAYER_H_

#define GRAV 3.0
#define JUMPFORCE 40.0

#include "Sprite.h"
#include "Character.h"
#include <vector>
#include <array>

class Hookshot;
class Wig;

class PlatformPlayer : public Character
{
public:
	PlatformPlayer(SDL_Rect s, SDL_FRect d, SDL_Renderer* r, SDL_Texture* t, int sstart = 0, int smin = 0, int smax = 0, int nf = 0);
	~PlatformPlayer();
	void Update(bool sX, bool sY);
	void Stop();
	void StopX();
	void StopY();
	void KnockLeft(double vel);
	void SetAccelX(double a);
	void SetAccelY(double a);
	double GetAccelX();
	double GetAccelY();
	double GetThurst();
	bool IsGrounded();
	void SetGrounded(bool g);
	double GetVelX();
	double GetVelY();
	void SetVelY(double a);
	void SetX(float y);
	void SetY(float y);
	double GetX();
	virtual void Render();
	void SetGrav(double grav) { m_grav = grav; }
	void RemoveHookShot();
	
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

	bool getGrapplehook()
	{
		return m_grapplehook;
	}

	void setGrapplehook(bool g)
	{
		m_grapplehook = g;
	}

	void setMoveHook(bool h)
	{
		m_movehook = h;
	}

	void setHookshot();
	void snatch();

	Hookshot* getHookShot() { return m_hookShot; }


private:
	bool m_grounded;
	bool m_grapplehook = false;
	bool m_movehook = false;
	bool m_flipped;
	double m_accelX,
		m_accelY,
		m_velX,
		m_maxVelX,
		m_velY,
		m_maxVelY,
		m_drag,
		m_thrust,
		m_grav;
	int m_wigCount = 0;
	int m_ShipParts = 0;
	float m_distance = 0;
	Hookshot* m_hookShot;

	std::vector<Wig*> m_vecwigCollection;

	
};

#endif