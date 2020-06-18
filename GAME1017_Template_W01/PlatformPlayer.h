#pragma once
#ifndef _PLATFORMPLAYER_H_
#define _PLATFORMPLAYER_H_
#define GRAV 6.0
#define JUMPFORCE 80.0

#include "Sprite.h"
#include "Character.h"

class PlatformPlayer : public Character
{
public:
	PlatformPlayer(SDL_Rect s, SDL_FRect d, SDL_Renderer* r, SDL_Texture* t, int sstart = 0, int smin = 0, int smax = 0, int nf = 0);
	void Update();
	void Stop();
	void StopX();
	void StopY();
	void KnockLeft(double vel);
	void SetAccelX(double a);
	void SetAccelY(double a);
	double GetAccelX();
	double GetAccelY();
	double GetThrust();
	bool IsGrounded();
	void SetGrounded(bool g);
	double GetVelX();
	double GetVelY();
	void SetVelY(double a);
	void SetX(float y);
	void SetY(float y);
	double GetX();
	double GetY();

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
private:
	bool m_grounded;
	double m_accelX,
		m_accelY,
		m_velX,
		m_maxVelX,
		m_velY,
		m_maxVelY,
		m_drag,
		m_thrust,
		m_grav;
	int m_wigCount;
	int m_ShipParts;
};

#endif