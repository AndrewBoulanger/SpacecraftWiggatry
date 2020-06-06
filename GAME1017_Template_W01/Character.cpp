#include "Character.h"

Character::Character(SDL_Rect s, SDL_FRect d, SDL_Renderer* r, SDL_Texture* t)
	: Sprite(s, d, r, t)
{
	health = 50;
	speed = 10;
	baseDamage = 10;
}