#pragma once
#include "Character.h"
#include "Pickup.h"
#include "TextureManager.h"

enum enemyState
{
    idle, seeking, fleeing
};

class Enemy :
    public Character
{
private:
    bool hasWig;
    Wig* enemysWig;
    int EnemyHP = 3;
    enemyState state;

    SDL_FRect wallWisker;
    SDL_FRect gapWisker;

protected:

public:
    Enemy(SDL_Rect s, SDL_FRect d, SDL_Renderer* r, SDL_Texture* t, int sstart = 0, int smin = 0, int smax = 0, int nf = 0, int hp = 2, int dmg = 1);
    ~Enemy();

    void setState(enemyState nState);

    bool getHasWig()
    {return hasWig; }
    Wig* getenemysWig()
    { 
        if(hasWig)
        return enemysWig; 
    }
    int getEnemyHP() { return EnemyHP; }
    void setEnemyHP(int h) { EnemyHP = h; }
    Wig* removeWig()
    {
        hasWig = false;
        return enemysWig;
    }


    void Update();
    void Render() override;

    void groundedMove2(const int dir);
};

