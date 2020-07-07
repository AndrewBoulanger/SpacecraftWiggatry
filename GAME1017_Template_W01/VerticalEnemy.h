#pragma once
#include "Enemy.h"
class VerticalEnemy :
    public Enemy
{
    VerticalEnemy(SDL_Rect s, SDL_FRect d, SDL_Renderer* r, SDL_Texture* t, int sstart, int smin, int smax, int nf, int hp, int dmg);

};

