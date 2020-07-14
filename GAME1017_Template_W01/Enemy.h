#pragma once
#include "Character.h"
#include "Pickup.h"
#include "TextureManager.h"
#include "raycast.h"

enum enemyState
{
    idle, seeking, fleeing
};

class Enemy :
    public Character
{
private:

    SDL_FRect wallWisker;
    SDL_FRect gapWisker;

protected:
    bool hasWig;
    Wig* enemysWig;
    enemyState state;
    raycast* sight;
    int lookTimer, losMax;

public:
    Enemy(SDL_Rect s, SDL_FRect d, SDL_Renderer* r, SDL_Texture* t, int sstart = 0, int smin = 0, int smax = 0, int nf = 0, int hp = 3, int dmg = 1);

    void setState(enemyState nState);

    virtual bool getHasWig()
    {return hasWig; }
    virtual Wig* getenemysWig()
    { 
        if(hasWig)
        return enemysWig; 
    }

    virtual Wig* removeWig();
    virtual void takeDamage(int dmg);

    void Update();
    void LOSCheck();
    virtual void Render() override;

    void groundedMove2(const int dir);
};

