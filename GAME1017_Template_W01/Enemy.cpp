#include "Enemy.h"

Enemy::Enemy(SDL_Rect s, SDL_FRect d, SDL_Renderer* r, SDL_Texture* t, int hp, int dmg)
	:Character(s, d, r, t)
{
	health = hp;
	baseDamage = dmg;
}