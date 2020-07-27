#pragma once
#include "Enemy.h"
class Boss :
    public Enemy
{
protected:
    int stateTimer, actionTimer;
    bool hasSpareWig;
public:
    Boss(SDL_Rect s, SDL_FRect d, SDL_Renderer* r, SDL_Texture* t, int sstart = 0, int smin = 0, int smax = 0, int nf = 0, int hp = 5, int dmg = 2);

    void Update();

    void Move();
    void Shoot();

    bool isShooting;

    Wig* removeWig() override;
};

