#pragma once
#include "Enemy.h"
class Boss :
    public Enemy
{
protected:

    bool hasSpareWig;
public:
    Boss(SDL_Rect s, SDL_FRect d, SDL_Renderer* r, SDL_Texture* t, int sstart = 0, int smin = 0, int smax = 0, int nf = 0, int hp = 5, int dmg = 2);

    void Update();
    void verticalMove(const int plrY);

    Wig* removeWig() override;
};

