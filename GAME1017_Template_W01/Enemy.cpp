#include "Enemy.h"

Enemy::Enemy(SDL_Rect s, SDL_FRect d, SDL_Renderer* r, SDL_Texture* t, int sstart, int smin, int smax, int nf, int hp, int dmg)
	:Character(s, d, r, t, sstart, smin, smax, nf, hp, dmg) {}