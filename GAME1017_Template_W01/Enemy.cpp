#include "Enemy.h"

Enemy::Enemy(SDL_Rect s, SDL_FRect d, SDL_Renderer* r, SDL_Texture* t, int hp, int spd, int dmg)
	:Character(s, d, r, t)
{
	health = hp;
	speed = spd;
	baseDamage = dmg;
}