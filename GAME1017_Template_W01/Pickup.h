#pragma once
#include "Sprite.h"
#include <time.h>

enum pickupType
{
    WIG,
    SHIP_PART
};

class Pickup :
    public Sprite
{
protected:
    pickupType type;

public:
    Pickup(SDL_Rect s, SDL_FRect d, SDL_Renderer* r, SDL_Texture* t) 
    : Sprite(s,d,r,t) {};
    virtual void Update()
    {
        m_dst.y += sinf(SDL_GetTicks()/200);
    };
    pickupType getType()
    {
        return type;
    };
};


//subclasses

class Wig : public Pickup
{
private:
    

public:
    Wig(SDL_Rect s, SDL_FRect d, SDL_Renderer* r, SDL_Texture* t)
        : Pickup(s, d, r, t)
    {
        type = WIG;
    };
};


class ShipPart : public Pickup
{
private:

public:
    ShipPart(SDL_Rect s, SDL_FRect d, SDL_Renderer* r, SDL_Texture* t)
        : Pickup(s, d, r, t)
    {
        type = SHIP_PART;
    };
};

