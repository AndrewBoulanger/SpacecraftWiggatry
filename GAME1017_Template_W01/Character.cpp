#include "Character.h"
#include <iostream>

using namespace std;

Character::Character(SDL_Rect s, SDL_FRect d, SDL_Renderer* r, SDL_Texture* t, int sstart, int smin, int smax, int nf, int hp, int dmg)
	: AnimatedSprite(s, d, r, t, sstart, smin,smax, nf), health(hp), baseDamage(dmg) {}


void Character::takeDamage(int dmg)
{
	health -= dmg;
	cout << "Health: " << health << endl;
}

void Character::StopX() { m_velX = 0.0; }
void Character::StopY() { m_velY = 0.0; }
	 
void Character::SetAccelX(double a) { m_accelX = a; }
void Character::SetAccelY(double a) { m_accelY = a; }
void Character::SetVelY(double a) { m_velY = a; }
double Character::GetVelX() { return m_velX; }
double Character::GetVelY() { return m_velY; }
double Character::GetAccelY() { return m_accelY; }
double Character::GetAccelX() { return m_accelX; }