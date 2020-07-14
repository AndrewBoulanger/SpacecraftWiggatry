#pragma once
#include "Sprite.h"
#include "MathManager.h"
class raycast
{
private:
	SDL_FPoint m_pos;
	double dX, dY, accelX, accelY, m_velMax, m_angle;

public:
	SDL_FPoint getPos() { return m_pos; }
	void setPos(float x, float y) { m_pos.x = x, m_pos.y = y; }
	void setX(float x) { m_pos.x = x; }
	void setY(float y) { m_pos.y = y; }
	void stop() { dX = dY = 0; }
	bool Update();

	void move2stop(double angle);
};

