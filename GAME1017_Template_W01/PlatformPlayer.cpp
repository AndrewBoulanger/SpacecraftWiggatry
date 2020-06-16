#include "PlatformPlayer.h"
#include "Engine.h"
#include "StateManager.h"
#include "TextureManager.h"
#include <algorithm>
#include <iostream>
#include "HookShot.h"

PlatformPlayer::PlatformPlayer(SDL_Rect s, SDL_FRect d, SDL_Renderer * r, SDL_Texture * t, int sstart, int smin, int smax, int nf)
	:Character(s, d, r, t, sstart, smin, smax, nf)
{
	m_grounded = true;
	m_accelX = m_accelY = m_velX = m_velY = 0.0;
	m_maxVelX = 10.0;
	m_maxVelY = JUMPFORCE;
	m_thrust = -GRAV;
	m_grav = GRAV;
	m_drag = 0.88;

	health = 50;
	baseDamage = 10;
	m_wigCount = 0;
	m_ShipParts = 0;
	m_hookShot = new Hookshot({ 0,0,36,36 }, { d.x, d.y, 36, 36 }, r, TEMA::GetTexture("hookshot"));

	// SDL_Rect src, SDL_FRect dst, SDL_Renderer* r, SDL_Texture* t
}

void PlatformPlayer::Update()
{
	// Do X axis first.
	m_velX += m_accelX;
	m_velX *= (m_grounded?m_drag:1); 
	m_velX = std::min(std::max(m_velX, -(m_maxVelX)), (m_maxVelX));
	m_dst.x += (int)m_velX; // Had to cast it to int to get crisp collision with side of platform.
	// Now do Y axis.
	m_velY += m_accelY + m_grav; // Adjust gravity to get slower jump.
	m_velY = std::min(std::max(m_velY, -(m_maxVelY)), (m_grav*5));
	m_dst.y += (int)m_velY; // To remove aliasing, I made cast it to an int too.
	m_accelX = m_accelY = 0.0;

	if (iCooldown > 0) 
		{ --iCooldown; }

	if (m_movehook)
	{
		m_hookShot->Update();
	}
}

void PlatformPlayer::Stop() // If you want a dead stop both axes.
{
	StopX();
	StopY();
}
void PlatformPlayer::StopX() { m_velX = 0.0; }
void PlatformPlayer::StopY() { m_velY = 0.0; }
void PlatformPlayer::KnockLeft(double vel) { m_velX -= vel; }

void PlatformPlayer::SetAccelX(double a) { m_accelX = a; }
void PlatformPlayer::SetAccelY(double a) { m_accelY = a; }
double PlatformPlayer::GetAccelX() { return m_accelX; }
double PlatformPlayer::GetAccelY() { return m_accelY; }
bool PlatformPlayer::IsGrounded() { return m_grounded; }
void PlatformPlayer::SetGrounded(bool g) { m_grounded = g; }
double PlatformPlayer::GetVelX() { return m_velX; }
double PlatformPlayer::GetVelY() { return m_velY; }
void PlatformPlayer::SetVelY(double a) { m_velY = a; }
void PlatformPlayer::SetX(float y) { m_dst.x = y; }
void PlatformPlayer::SetY(float y) { m_dst.y = y; }
double PlatformPlayer::GetX() { return m_dst.x; }

void PlatformPlayer::Render()
{ 
	SDL_RenderCopyExF(m_pRend, m_pText, GetSrcP(), GetDstP(), m_angle, 0, SDL_FLIP_NONE);
	
	if (m_grapplehook)
	{
		m_hookShot->Render();
	}
}

double PlatformPlayer::GetThurst() { return m_thrust; }

void PlatformPlayer::takeDamage(int dmg)
{
	if (iCooldown <= 0)
	{
		health -= dmg;
		iCooldown = iFrames;
		std::cout << "Health: " << health << std::endl;
	}
	if (health <= 0)
		STMA::ChangeState(new DeadState);
}

void PlatformPlayer::setHookshot()
{
	SDL_Point point;
	point.x = m_dst.x + m_dst.w * 0.5;
	point.y = m_dst.y + m_dst.h * 0.5;
	//m_hookShot->SetPos(point);
	m_hookShot->sethookFixed(false);
}

void PlatformPlayer::snatch()
{
	SDL_FRect* EnemyDstP = ((GameState*)(STMA::GetStates().back()))->getEnemy()->GetDstP();
	float PlayerX = m_dst.x + (m_dst.w * 0.5);
	float PlayerY = m_dst.y + (m_dst.h * 0.5);
	float EnemyX = EnemyDstP->x + (EnemyDstP->w * 0.5);
	float EnemyY = EnemyDstP->y + (EnemyDstP->h * 0.5);

	m_distance = sqrtf(((PlayerX - EnemyX) * (PlayerX - EnemyX)) + ((PlayerY - EnemyY) * (PlayerY - EnemyY)));

	if (m_distance < 100)
	{
		Enemy* Enemy = ((GameState*)(STMA::GetStates().back()))->getEnemy();
		Enemy->setHasWig(false);
		m_vecwigCollection.push_back(Enemy->getenemysWig());
	}
}
