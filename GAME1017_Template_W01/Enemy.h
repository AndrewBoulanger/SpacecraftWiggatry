#pragma once
#include "Character.h"
class Enemy :
    public Character
{
private:

protected:

public:
    Enemy(SDL_Rect s, SDL_FRect d, SDL_Renderer* r, SDL_Texture* t, int hp, int spd, int dmg);


};

