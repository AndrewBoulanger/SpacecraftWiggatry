#pragma once
#include "Sprite.h"

enum bulletType
{
	PLAYER_BULLET,
	ENEMY_BULLET
};

class Projectile : public AnimatedSprite
{
private:

	float m_speed = 15.0f;
	float velX = 0;
	float velY = 0;
	float m_size;

	bulletType owner;

public:
	Projectile(bool fromPlayer, SDL_FPoint origin, double angle, float size, SDL_Texture* t);
	Projectile(SDL_Rect src, SDL_FRect dst, SDL_Renderer* r, SDL_Texture* t);
	~Projectile();
	void setVelX(float x) { velX = x; }
	void setVelY(float y) { velY = y; }
	void update();
	void Render() override;

	bulletType getOwner() { return owner; };
};