#pragma once
#include "Sprite.h"

class Hookshot : public Sprite
{
private:
	
	float velX = 0;
	float velY = 0;
	float speed = 50.0f;
	float shotAngle = 0;
	float lerpCo = 0;
	bool hookFixed = false;
	SDL_FRect line;
	SDL_FRect* playerdst;
	SDL_FPoint targetPos;

public:
	Hookshot(SDL_Rect src, SDL_FRect dst, SDL_Renderer* r, SDL_Texture* t);
	~Hookshot();

	float getVelX() { return velX; }
	float getVelY() { return velY; }

	void calHookAngle(SDL_FRect* playerPos);
	void move();
	void Collision();
	float MyLerp(float a, float b, float t);
	void Update(double& grav);
	void Render();
	bool gethookFixed() { return hookFixed; }
	void sethookFixed(bool b);
	void setlerpCo(float lc);

};