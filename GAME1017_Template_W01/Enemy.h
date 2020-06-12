#pragma once
#include "Character.h"
class Enemy :
    public Character
{
private:

protected:

public:
    Enemy(SDL_Rect s, SDL_FRect d, SDL_Renderer* r, SDL_Texture* t, int sstart = 0, int smin = 0, int smax = 0, int nf = 0, int hp = 20, int dmg = 10);


};

