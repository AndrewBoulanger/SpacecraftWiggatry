#pragma once
#include "Sprite.h"

class StunGun : public Sprite
{
private:

	float m_speed = 5.0f;
	float velX = 0;
	float velY = 0;
	bool m_active = true;

public:
	StunGun(SDL_Rect src, SDL_FRect dst, SDL_Renderer* r, SDL_Texture* t);
	~StunGun();
	void setVelX(float x) { velX = x; }
	void setVelY(float y) { velY = y; }
	void update();
	void render();
};