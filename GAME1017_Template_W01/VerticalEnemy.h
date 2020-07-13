#pragma once
#include "Enemy.h"
class VerticalEnemy :
    public Enemy
{
private:

public:
    VerticalEnemy(SDL_Rect s, SDL_FRect d, SDL_Renderer* r, SDL_Texture* t, int sstart = 0, int smin = 0, int smax = 0, int nf = 0, int hp = 3, int dmg = 1);
 //   VerticalEnemy(SDL_Rect s, SDL_FRect d, SDL_Renderer* r, SDL_Texture* t, int sstart = 0, int smin = 0, int smax = 0, int nf = 0, int hp = 2, int dmg = 1);
    void Update();
    void verticalMove(const int plrY);
};

