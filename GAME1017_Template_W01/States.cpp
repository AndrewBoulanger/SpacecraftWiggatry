#include "States.h"
#include "CollisionManager.h"
#include "EventManager.h"
#include "SoundManager.h"
#include "StateManager.h" // Make sure this is NOT in "States.h" or circular reference.
#include "TextureManager.h"
#include "Engine.h"
#include "Button.h"
#include "Enemy.h"
#include <iostream>

// Begin State. CTRL+M+H and CTRL+M+U to turn on/off collapsed code.
void State::Render()
{
	SDL_RenderPresent(Engine::Instance().GetRenderer());
}
void State::Resume() {}
// End State.

// Begin GameState.
GameState::GameState() {}

void GameState::Enter()
{
	std::cout << "Entering GameState..." << std::endl;
	m_pPlayer = new PlatformPlayer({ 0,0,400,100 }, { 512.0f,548.0f,50.0f,100.0f }, 
								   Engine::Instance().GetRenderer(), TEMA::GetTexture("player"));
	m_pEnemy = new Enemy({ 0,0,400,100 }, {600.0f, 500.0f, 50.0f, 100.0f}, Engine::Instance().GetRenderer(), TEMA::GetTexture("enemy"), 10, 10);
	m_pPlatforms[0] = new SDL_FRect({ 462.0f,648.0f,100.0f,20.0f });
	m_pPlatforms[1] = new SDL_FRect({ 200.0f,468.0f,100.0f,20.0f });
	m_pPlatforms[2] = new SDL_FRect({ 724.0f,468.0f,100.0f,20.0f });
	m_pPlatforms[3] = new SDL_FRect({ 462.0f,368.0f,100.0f,20.0f });
	m_pPlatforms[4] = new SDL_FRect({ -100.0f,668.0f,1224.0f,100.0f });
	SOMA::Load("Aud/jump.wav", "jump", SOUND_SFX);
}

void GameState::Update()
{
	// Get input.
	if (EVMA::KeyHeld(SDL_SCANCODE_A))
		m_pPlayer->SetAccelX(-1.0);
	else if (EVMA::KeyHeld(SDL_SCANCODE_D))
		m_pPlayer->SetAccelX(1.0);

	if (EVMA::KeyHeld(SDL_SCANCODE_SPACE) && !m_pPlayer->IsGrounded())
	{
		if (m_pPlayer->GetVelY() >= 0)
		{
			m_pPlayer->SetAccelY(m_pPlayer->GetThurst());
			m_pPlayer->SetVelY(0);
		}

	}
	if (EVMA::KeyPressed(SDL_SCANCODE_SPACE) && m_pPlayer->IsGrounded())
	{
		SOMA::PlaySound("jump");
		m_pPlayer->SetAccelY(-JUMPFORCE); // Sets the jump force.
		m_pPlayer->SetGrounded(false);
	}


	// Wrap the player on screen.
	if (m_pPlayer->GetDstP()->x < -51.0) m_pPlayer->SetX(1024.0);
	else if (m_pPlayer->GetDstP()->x > 1024.0) m_pPlayer->SetX(-50.0);
	// Do the rest.
	m_pPlayer->Update();
	CheckCollision();
	if (EVMA::KeyPressed(SDL_SCANCODE_P))
	{
		STMA::PushState(new PauseState);
	}
	if (m_pPlayer->getHealth() < 1)
		STMA::ChangeState(new DeadState);
}

void GameState::CheckCollision()
{
	for (int i = 0; i < NUMPLATFORMS; i++) // For each platform.
	{
		if (COMA::AABBCheck(*m_pPlayer->GetDstP(), *m_pPlatforms[i]))
		{
			if (m_pPlayer->GetDstP()->x + m_pPlayer->GetDstP()->w - m_pPlayer->GetVelX() <= m_pPlatforms[i]->x)
			{ // Collision from left.
				m_pPlayer->StopX(); // Stop the player from moving horizontally.
				m_pPlayer->SetX(m_pPlatforms[i]->x - m_pPlayer->GetDstP()->w);
			}
			else if (m_pPlayer->GetDstP()->x - (float)m_pPlayer->GetVelX() >= m_pPlatforms[i]->x + m_pPlatforms[i]->w)
			{ // Colliding right side of platform.
				m_pPlayer->StopX();
				m_pPlayer->SetX(m_pPlatforms[i]->x + m_pPlatforms[i]->w);
			}
			else if (m_pPlayer->GetDstP()->y + m_pPlayer->GetDstP()->h - (float)m_pPlayer->GetVelY() <= m_pPlatforms[i]->y)
			{ // Colliding top side of platform.
				m_pPlayer->SetGrounded(true);
				m_pPlayer->StopY();
				m_pPlayer->SetY(m_pPlatforms[i]->y - m_pPlayer->GetDstP()->h - 1);
			}
			else if (m_pPlayer->GetDstP()->y - (float)m_pPlayer->GetVelY() >= m_pPlatforms[i]->y + m_pPlatforms[i]->h)
			{ // Colliding bottom side of platform.
				m_pPlayer->StopY();
				m_pPlayer->SetY(m_pPlatforms[i]->y + m_pPlatforms[i]->h);
			}
		}
	}

	if (COMA::AABBCheck(*m_pPlayer->GetDstP(), *m_pEnemy->GetDstP()))
	{
		if (m_pPlayer->GetDstP()->x - (float)m_pPlayer->GetVelX() >= m_pEnemy->GetDstP()->x + m_pEnemy->GetDstP()->w)
		{ // Colliding right side of platform.
			m_pPlayer->StopX();
			m_pPlayer->KnockLeft(-10);
		}
		else
		{
			m_pPlayer->Stop();
			m_pPlayer->KnockLeft(10);
		}
		m_pPlayer->takeDamage(m_pEnemy->getBaseDamage());
	}
}

void GameState::Render()
{
	SDL_SetRenderDrawColor(Engine::Instance().GetRenderer(), 64, 128, 255, 255);
	SDL_RenderClear(Engine::Instance().GetRenderer());
	//draw the enemy
	m_pEnemy->Render();
	// Draw the player.
	m_pPlayer->Render();
	// Draw the platforms.
	SDL_SetRenderDrawColor(Engine::Instance().GetRenderer(), 192, 64, 0, 255);
	for (int i = 0; i < NUMPLATFORMS; i++)
		SDL_RenderFillRectF(Engine::Instance().GetRenderer(), m_pPlatforms[i]);
	// If GameState != current state.
	if (dynamic_cast<GameState*>(STMA::GetStates().back()))
		State::Render();
}

void GameState::Exit()
{
	delete m_pPlayer;
	for (int i = 0; i < NUMPLATFORMS; i++)
		delete m_pPlatforms[i];
}

void GameState::Resume() { }
// End GameState.

// Begin TitleState.
TitleState::TitleState() {}

void TitleState::Enter()
{
	m_playBtn = new PlayButton({ 0,0,400,100 }, { 312.0f,400.0f,400.0f,100.0f }, Engine::Instance().GetRenderer(), TEMA::GetTexture("play"));
	m_quitBtn = new QuitButton({ 0,0,400,100 }, { 312.0f,520.0f,400.0f,100.0f }, Engine::Instance().GetRenderer(), TEMA::GetTexture("exit"));
	SOMA::Load("Aud/power.wav", "beep", SOUND_SFX);
}

void TitleState::Update()
{
	if (m_playBtn->Update() == 1)
		return; 
	if (m_quitBtn->Update() == 1)
		return;
}

void TitleState::Render()
{
	SDL_SetRenderDrawColor(Engine::Instance().GetRenderer(), 255, 51, 255, 0);
	SDL_RenderClear(Engine::Instance().GetRenderer());
	m_playBtn->Render();
	m_quitBtn->Render();
	State::Render();
}

void TitleState::Exit()
{
	std::cout << "Exiting TitleState..." << std::endl;
}
// End TitleState.

// Pause StateStuff
PauseState::PauseState() {}

void PauseState::Enter()
{
	std::cout << "Entering Pause...\n";
	m_resumeBtn = new ResumeButton({ 0,0,200,80 }, { 415.0f,400.0f,200.0f,80.0f }, Engine::Instance().GetRenderer(), TEMA::GetTexture("resume"));//1024
}

void PauseState::Update()
{
	if (m_resumeBtn->Update() == 1)
		return;
}

void PauseState::Render()
{
	STMA::GetStates().front()->Render();
	SDL_SetRenderDrawBlendMode(Engine::Instance().GetRenderer(), SDL_BLENDMODE_BLEND); // below won't be taken into account unles we do this // blendmode create transparency
	SDL_SetRenderDrawColor(Engine::Instance().GetRenderer(), 255, 51, 225, 100);
	SDL_Rect rect = { 173, 128, 700, 512 };
	SDL_RenderFillRect(Engine::Instance().GetRenderer(), &rect);
	m_resumeBtn->Render();
	State::Render();
}

void PauseState::Exit()
{
	std::cout << "Exiting PauseState...\n";
}
// End of PauseState

// Begin DeadState.
DeadState::DeadState() {}

void DeadState::Enter()
{
	std::cout << "Entering DeadState...\n";
	m_playBtn = new PlayButton({ 0,0,400,100 }, { 312.0f,400.0f,400.0f,100.0f }, Engine::Instance().GetRenderer(), TEMA::GetTexture("play"));
	m_quitBtn = new QuitButton({ 0,0,400,100 }, { 312.0f,520.0f,400.0f,100.0f }, Engine::Instance().GetRenderer(), TEMA::GetTexture("exit"));
	SOMA::Load("Aud/power.wav", "beep", SOUND_SFX);
}

void DeadState::Update()
{
	if (m_playBtn->Update() == 1)
		return;
	if (m_quitBtn->Update() == 1)
		return;
}

void DeadState::Render()
{
	SDL_SetRenderDrawColor(Engine::Instance().GetRenderer(), 255, 0, 0, 0);
	SDL_RenderClear(Engine::Instance().GetRenderer());
	m_playBtn->Render();
	m_quitBtn->Render();
	State::Render();
}

void DeadState::Exit()
{
	std::cout << "Exiting DeadState...a" << std::endl;
}
// End DeadState.