#include "Character.h"
#include <iostream>

using namespace std;

Character::Character(SDL_Rect s, SDL_FRect d, SDL_Renderer* r, SDL_Texture* t, int sstart, int smin, int smax, int nf, int hp, int dmg)
	: AnimatedSprite(s, d, r, t, sstart, smin,smax, nf), health(hp), baseDamage(dmg) {}

void Character::Render()
{
	SDL_RenderCopyF(m_pRend, m_pText, &m_src, &m_dst);
}

void Character::takeDamage(int dmg)
{
	health -= dmg;
	cout << "Health: " << health << endl;
}