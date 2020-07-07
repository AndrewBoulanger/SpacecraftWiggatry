#include "States.h"
#include "CollisionManager.h"
#include "EventManager.h"
#include "SoundManager.h"
#include "StateManager.h" // Make sure this is NOT in "States.h" or circular reference.
#include "TextureManager.h"
#include "FontManager.h"
#include "MathManager.h"
#include "Engine.h"
#include "Button.h"
#include "Enemy.h"
#include "PlatformPlayer.h"
#include "HookShot.h"
#include <iostream>
#include <fstream>
#include <string>
#include "SpriteManager.h"
#include "Utilities.h"


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
	// FOMA::SetSize("Img/font.ttf", "font", 35); not working DX
	m_pPlayer = SPMR::getPlayer();

	m_pauseBtn = new PauseButton({ 0,0,86,78 }, { 1005.0f,0.0f,21.5f,19.5f }, Engine::Instance().GetRenderer(), TEMA::GetTexture("pause"));
	m_pReticle = new Sprite({ 0,0, 36,36 }, { 0,0, 25,25 }, Engine::Instance().GetRenderer(), TEMA::GetTexture("reticle"));

	// ui stuff
	for (int i = 0; i < (5); i++)
		hpUI[i] = new Sprite({ 0,0, 256,256 }, { (float)(35*i),0, 35,35 }, Engine::Instance().GetRenderer(), TEMA::GetTexture("heart"));
	for (int i = 0; i < (5); i++)
		stungunUI[i] = new Sprite({ 0,0, 29,35 }, { (float)(35 * i),36, 29,32 }, Engine::Instance().GetRenderer(), TEMA::GetTexture("lightning"));
	wigUI = new Sprite({ 0,0, 100,100 }, { (float)(185),0, 35,35 }, Engine::Instance().GetRenderer(), TEMA::GetTexture("wig"));
	shipUI = new Sprite({ 0,0, 74, 75 }, { (float)(250),-3, 35,33 }, Engine::Instance().GetRenderer(), TEMA::GetTexture("shippart"));
	words[0] = new Label("font", 225, 4, to_string((int)(m_pPlayer->getWigs())).c_str(), { 255,255,255,0 });
	words[1] = new Label("font", 289, 4, to_string((int)(m_pPlayer->getParts())).c_str(), { 255,255,255,0 });

	gameOver = false; 
	timeToSwitchLevels = false;

	m_pPlayer->SetPos({ (int)100.0f,(int)600.0f });

	if (Engine::Instance().getLevel() == 1)
	{
		// loading first level
		Engine::LoadLevel("Dat/Level1.txt");
	
		SPMR::PushSprite(new Enemy({ 0,0,400,140 }, { 850.0f, 500.0f, 50.0f, 106.0f },
				Engine::Instance().GetRenderer(), TEMA::GetTexture("enemy"), 3, 1));
		SPMR::PushSprite(new Enemy({ 0,0,400,140 }, { 1730.0f, 200.0f, 50.0f, 106.0f },
			Engine::Instance().GetRenderer(), TEMA::GetTexture("enemy"), 3, 1));
		SPMR::PushSprite(new Enemy({ 0,0,400,140 }, {2700.0f, 100.0f, 50.0f, 106.0f },
			Engine::Instance().GetRenderer(), TEMA::GetTexture("enemy"), 3, 1));
		SPMR::PushSprite(new Enemy({ 0,0,400,140 }, { 3900.0f, 500.0f, 50.0f, 106.0f },
			Engine::Instance().GetRenderer(), TEMA::GetTexture("enemy"), 3, 1));

			SPMR::PushSprite(new Wig({ 0,0,100,100 }, { 600.0f, 400.0f,50.0f,50.0f },
				Engine::Instance().GetRenderer(), TEMA::GetTexture("wig")));

			SPMR::PushSprite(new Energy({ 0,0,100,100 }, { 3700.0f, 100.0f,45.0f,45.0f },
				Engine::Instance().GetRenderer(), TEMA::GetTexture("lightning")));

			SPMR::PushSprite(new Energy({ 0,0,256,256 }, { 4100.0f, 400.0f,50.0f,50.0f },
				Engine::Instance().GetRenderer(), TEMA::GetTexture("heart")));

		m_pEnemy = SPMR::GetEnemies()[0];
		m_pEnemy->setHealth(3);  //this is just here to test snatching

		m_pPlayer->setHealth(5);
	m_flag = new Sprite({ 0,0, 32, 64 }, { (32 * 137) , (32 * 20), 32, 64 }, Engine::Instance().GetRenderer(), TEMA::GetTexture("flag"));
	}
	if (Engine::Instance().getLevel() == 2)
	{
		Engine::LoadLevel("Dat/Level2.txt");

		SPMR::PushSprite(new Enemy({ 0,0,400,140 }, { 500.0f, 200.0f, 50.0f, 106.0f },
			Engine::Instance().GetRenderer(), TEMA::GetTexture("enemy2"), 4, 1));
		SPMR::PushSprite(new Enemy({ 0,0,400,140 }, { 1600.0f, 600.0f, 50.0f, 106.0f },
			Engine::Instance().GetRenderer(), TEMA::GetTexture("enemy2"), 4, 1));
		SPMR::PushSprite(new Enemy({ 0,0,400,140 }, { 3400.0f, 100.0f, 50.0f, 106.0f },
			Engine::Instance().GetRenderer(), TEMA::GetTexture("enemy2"), 4, 1));
		SPMR::PushSprite(new Enemy({ 0,0,400,140 }, { 4200.0f, 30.0f, 50.0f, 106.0f },
			Engine::Instance().GetRenderer(), TEMA::GetTexture("enemy2"), 4, 1));

		// ship parts of lvl 2
		SPMR::PushSprite(new ShipPart({ 0,0,74, 75 }, { (32.0f * 14.0f), (32.0f * 14.5f), 50.0f, 50.0f },
			Engine::Instance().GetRenderer(), TEMA::GetTexture("shippart")));
		SPMR::PushSprite(new ShipPart({ 0,0,74, 75 }, { (32.0f * 36.0f), (32.0f * 8.5f), 50.0f, 50.0f },
			Engine::Instance().GetRenderer(), TEMA::GetTexture("shippart")));
		SPMR::PushSprite(new ShipPart({ 0,0,74, 75 }, { (32.0f * 69.0f), (32.0f * 7.5f), 50.0f, 50.0f },
			Engine::Instance().GetRenderer(), TEMA::GetTexture("shippart")));
		SPMR::PushSprite(new ShipPart({ 0,0,74, 75 }, { (32.0f * 136.0f), (32.0f * 2.5f), 50.0f, 50.0f },
			Engine::Instance().GetRenderer(), TEMA::GetTexture("shippart")));
		SPMR::PushSprite(new ShipPart({ 0,0,74, 75 }, { (32.0f * 123.0f), (32.0f * 13.5f), 50.0f, 50.0f },
			Engine::Instance().GetRenderer(), TEMA::GetTexture("shippart")));
	
	m_flag = new Sprite({ 0,0, 32, 64 }, { (32 * 137) , (32 * 20), 32, 64 }, Engine::Instance().GetRenderer(), TEMA::GetTexture("flag"));
	}

	SPMR::PushSprite(m_flag, Regular);
		SOMA::PlayMusic("PokerFace");

}

void GameState::Update()
{
	m_pReticle->SetPos(EVMA::GetMousePos());

	// Pause button/key
	if (EVMA::KeyPressed(SDL_SCANCODE_P))
	{
		STMA::PushState(new PauseState);
	}
	if (m_pauseBtn->ButtonUpdate() == 1)
		return;

	SPMR::Update();

	// UI
	words[0]->SetText(to_string((int)(m_pPlayer->getWigs())).c_str());
	words[1]->SetText(to_string((int)(m_pPlayer->getParts())).c_str()); // change to ship!!!!!!!!!!!!!!!1

	m_pPlayer->Update(); // Change to player Update here.
	CheckCollision();
}


void GameState::CheckCollision()
{	
	for(int i = 0; i < SPMR::GetEnemies().size();i++)
	{
		m_pEnemy = SPMR::GetEnemies()[i];
		if (COMA::CircleCircleCheck(m_pPlayer->getCenter(), m_pEnemy->getCenter(), 40))
		{
			if (m_pPlayer->GetDstP()->x - (float)m_pPlayer->GetVelX() >= m_pEnemy->GetDstP()->x + m_pEnemy->GetDstP()->w)
			{ // Colliding right side of platform.
				m_pPlayer->StopX();
				m_pPlayer->KnockLeft(-5); //knock the player to the right
			}
			else
			{
				m_pPlayer->Stop();
				m_pPlayer->KnockLeft(5);
			}
			m_pPlayer->takeDamage(m_pEnemy->getBaseDamage());
		}
	}
	m_pEnemy = nullptr;

	if (COMA::AABBCheck(*m_pPlayer->GetDstP(), *m_flag->GetDstP())) // TEMPORARY loading lvl here... messy   
	{                       //we could use the change state function to make a new game state, then we can move some of this stuff to the exit state and the stuff that's 
						//	different between levels could be put in an if statement in the enter function
		timeToSwitchLevels = true;
		if (Engine::Instance().getLevel() == 1)
			Engine::Instance().setLevel(2);
		else
			Engine::Instance().setLevel(1);

	}
	if (m_pPlayer->getHealth() <= 0)
	{
		gameOver = true;
		m_pPlayer->setHealth(5);// reset health for the next game, we can move this to enter if we want it to reset every level instead
	}
	if (gameOver)
		STMA::ChangeState(new DeadState);
	else if (timeToSwitchLevels)
		STMA::ChangeState(new GameState);
}

void GameState::Render()
{
	SDL_SetRenderDrawColor(Engine::Instance().GetRenderer(), 0, 0, 0, 255);
	SDL_RenderClear(Engine::Instance().GetRenderer());

	// flag goes behind player
	if (m_flag != nullptr)
		m_flag->Render();

	m_pReticle->Render();
	m_pauseBtn->Render();
	SPMR::Render();

	// ui stuff
	for (int i = 0; i < (m_pPlayer->getHealth()); i++)
		hpUI[i]->Render();
	for (int i = 0; i < (m_pPlayer->getEnergy()); i++) // TO: reference stun uses
		stungunUI[i]->Render();
	wigUI->Render();
	shipUI->Render();
	words[0]->Render();
	words[1]->Render();

	// If GameState != current state.
	if (dynamic_cast<GameState*>(STMA::GetStates().back()))
		State::Render();

}

void GameState::Exit()
{
	delete m_pReticle;
	m_pReticle = nullptr;

	SPMR::RemoveLevel();

	
	m_flag->readyToDelete = true;
	m_flag = nullptr;
	
}

void GameState::Resume() { }
// End GameState.

// Begin TitleState.
TitleState::TitleState() {}

void TitleState::Enter()
{
	m_playBtn = new PlayButton({ 0,0,400,100 }, { 312.0f,400.0f,400.0f,100.0f }, Engine::Instance().GetRenderer(), TEMA::GetTexture("play"));
	m_quitBtn = new QuitButton({ 0,0,400,100 }, { 312.0f,520.0f,400.0f,100.0f }, Engine::Instance().GetRenderer(), TEMA::GetTexture("exit"));
	m_controlsBtn = new ControlsButton({ 0,0,200,47 }, { 824.0f,721.0f,200.0f,47.0f }, Engine::Instance().GetRenderer(), TEMA::GetTexture("control"));
	instructions = new Sprite({ 0,0,525,350 }, { 260.5f,140.0f,525.0f,350.0f }, Engine::Instance().GetRenderer(), TEMA::GetTexture("controls"));
	background = new Sprite({ 0,0,1204,768 }, { 0.0f,0.0f,1204.0f,768.0f }, Engine::Instance().GetRenderer(), TEMA::GetTexture("bg"));
	words[0] = new Label("fontLarge", 180, 110, "SPACECRAFT", { 188,7,208,0 });
	words[1] = new Label("fontLarge", 260, 200, "Wiggatry", { 255,255,255,0 });
	words[2] = new Label("fontLarge", 0, 670, "ETTG", { 255,0,180,0 });
	words[3] = new Label("font", 370, 600, "Press ENTER to return", { 255,255,255,0 });
	SOMA::Load("Aud/power.wav", "beep", SOUND_SFX);
}

void TitleState::Update()
{
	if (displayControls == false)
	{
		if (m_playBtn->ButtonUpdate() == 1)
			return;
		if (m_quitBtn->ButtonUpdate() == 1)
			return;
		if (m_controlsBtn->ButtonUpdate() == 1)
		{
			displayControls = true;
			return;
		}
	}
	if (EVMA::KeyPressed(SDL_SCANCODE_RETURN))
	{
		displayControls = false;
	}
}

void TitleState::Render()
{
	SDL_SetRenderDrawColor(Engine::Instance().GetRenderer(), 255, 51, 255, 0);
	SDL_RenderClear(Engine::Instance().GetRenderer());
	background->Render();
	for (int i = 0; i < 3; i++)
		words[i]->Render();
	m_playBtn->Render();
	m_quitBtn->Render();
	m_controlsBtn->Render();

	if (displayControls == true)
	{
		SDL_SetRenderDrawColor(Engine::Instance().GetRenderer(), 255, 51, 225, 170);
		SDL_Rect rect = { 173, 128, 700, 512 };
		SDL_RenderFillRect(Engine::Instance().GetRenderer(), &rect);
		instructions->Render();
		words[3]->Render();
	}

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
	m_resumeBtn = new ResumeButton({ 0,0,200,80 }, { 415.0f,500.0f,200.0f,80.0f }, Engine::Instance().GetRenderer(), TEMA::GetTexture("resume"));//1024
	instructions = new Sprite({ 0,0,525,350 }, { 260.5f,140.0f,525.0f,350.0f },Engine::Instance().GetRenderer(), TEMA::GetTexture("controls"));
}

void PauseState::Update()
{
	if (m_resumeBtn->ButtonUpdate() == 1)
		return;
}

void PauseState::Render()
{
	STMA::GetStates().front()->Render();
	SDL_SetRenderDrawBlendMode(Engine::Instance().GetRenderer(), SDL_BLENDMODE_BLEND); // below won't be taken into account unles we do this // blendmode create transparency
	SDL_SetRenderDrawColor(Engine::Instance().GetRenderer(), 255, 51, 225, 170);
	SDL_Rect rect = { 173, 128, 700, 512 };
	SDL_RenderFillRect(Engine::Instance().GetRenderer(), &rect);
	instructions->Render();
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
	words = new Label("fontLarge", 220, 110, "GAME OVER", { 255,255,255,0 });
	m_playBtn = new PlayButton({ 0,0,400,100 }, { 312.0f,400.0f,400.0f,100.0f }, Engine::Instance().GetRenderer(), TEMA::GetTexture("replay"));
	m_quitBtn = new QuitButton({ 0,0,400,100 }, { 312.0f,520.0f,400.0f,100.0f }, Engine::Instance().GetRenderer(), TEMA::GetTexture("exit"));
	SOMA::Load("Aud/power.wav", "beep", SOUND_SFX);
	SOMA::PlayMusic("WreckingBall");
}

void DeadState::Update()
{
	if (m_playBtn->ButtonUpdate() == 1)
		return;
	if (m_quitBtn->ButtonUpdate() == 1)
		return;
}

void DeadState::Render()
{
	SDL_SetRenderDrawColor(Engine::Instance().GetRenderer(), 255, 0, 0, 0);
	SDL_RenderClear(Engine::Instance().GetRenderer());
	words->Render();
	m_playBtn->Render();
	m_quitBtn->Render();
	State::Render();
}

void DeadState::Exit()
{
	std::cout << "Exiting DeadState...a" << std::endl;
}
// End DeadState.