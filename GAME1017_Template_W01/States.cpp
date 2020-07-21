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
#include "VerticalEnemy.h"
#include "Boss.h"


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
	SDL_ShowCursor(SDL_DISABLE); // we have a reticle so...
	m_pPlayer = SPMR::getPlayer();

	m_pauseBtn = new PauseButton({ 0,0,86,78 }, { 1005.0f,0.0f,21.5f,19.5f }, Engine::Instance().GetRenderer(), TEMA::GetTexture("pause"));
	m_pReticle = new Sprite({ 0,0, 36,36 }, { 0,0, 25,25 }, Engine::Instance().GetRenderer(), TEMA::GetTexture("reticle"));

	wigCount = m_pPlayer->getWigs();
	partsCount = m_pPlayer->getParts();

	// ui stuff
	for (int i = 0; i < (5); i++)
		hpUI[i] = new Sprite({ 0,0, 256,256 }, { (float)(35*i),0, 35,35 }, Engine::Instance().GetRenderer(), TEMA::GetTexture("heart"));
	for (int i = 0; i < (5); i++)
		stungunUI[i] = new Sprite({ 0,0, 29,35 }, { (float)(35 * i),36, 29,32 }, Engine::Instance().GetRenderer(), TEMA::GetTexture("lightning"));
	wigUI = new Sprite({ 0,0, 100,100 }, { (float)(185),0, 35,35 }, Engine::Instance().GetRenderer(), TEMA::GetTexture("wig"));
	shipUI = new Sprite({ 0,0, 74, 75 }, { (float)(250),-3, 35,33 }, Engine::Instance().GetRenderer(), TEMA::GetTexture("shippart"));
	words[0] = new Label("font", 225, 4, to_string((int)(m_pPlayer->getWigs())).c_str(), { 255,255,255,0 });
	words[1] = new Label("font", 289, 4, to_string((int)(m_pPlayer->getParts())).c_str(), { 255,255,255,0 });
	words[2] = new Label("font", 15, 720, "objective updated base on level", { 255,255,255,0 });

	gameOver = false; 
	timeToSwitchLevels = false;

	m_pPlayer->SetPos({ (int)100.0f,(int)600.0f });

	if (Engine::Instance().getLevel() == 1)
	{
		Engine::LoadLevel("Dat/Level1.txt");
		words[2]->SetText("Reach the end of the map, collect as many wigs as you can");
	
		SPMR::PushSprite(new VerticalEnemy({ 0,0,55,140 }, { 900, 300, 50.0f, 106 }, Engine::Instance().GetRenderer(), TEMA::GetTexture("enemies")));
		SPMR::PushSprite(new VerticalEnemy({ 0,0,55,140 }, { 750.0f, 500.0f, 50.0f, 106.0f },
				Engine::Instance().GetRenderer(), TEMA::GetTexture("enemies"), 3, 1));
		SPMR::PushSprite(new Enemy({ 110,0,55,140 }, { 1710.0f, 200.0f, 50.0f, 106.0f },
			Engine::Instance().GetRenderer(), TEMA::GetTexture("enemies"), 3, 1));
		SPMR::PushSprite(new Enemy({ 110,0,55,140 }, {2700.0f, 100.0f, 50.0f, 106.0f },
			Engine::Instance().GetRenderer(), TEMA::GetTexture("enemies"), 3, 1));
		SPMR::PushSprite(new Enemy({ 110,0,55,140 }, { 3900.0f, 500.0f, 50.0f, 106.0f },
			Engine::Instance().GetRenderer(), TEMA::GetTexture("enemies"), 3, 1));

		SPMR::PushSprite(new Wig({ 0,0,100,100 }, { 600.0f, 400.0f,50.0f,50.0f },
			Engine::Instance().GetRenderer(), TEMA::GetTexture("wig")));

		SPMR::PushSprite(new Energy({ 0,0,100,100 }, { 3700.0f, 100.0f,45.0f,45.0f },
			Engine::Instance().GetRenderer(), TEMA::GetTexture("lightning")));

		SPMR::PushSprite(new Health({ 0,0,256,256 }, { 4100.0f, 400.0f,50.0f,50.0f },
			Engine::Instance().GetRenderer(), TEMA::GetTexture("heart")));

		m_pEnemy = SPMR::GetEnemies()[0];
		m_pEnemy->setHealth(3);  //this is just here to test snatching

		m_pPlayer->setHealth(5);
	m_flag = new Sprite({ 0,0, 32, 64 }, { (32 * 137) , (32 * 20), 32, 64 }, Engine::Instance().GetRenderer(), TEMA::GetTexture("flag"));
	}
	if (Engine::Instance().getLevel() == 2)
	{
		Engine::LoadLevel("Dat/Level2.txt");
		words[2]->SetText("Collect at least 5 ship parts and reach the end of the map");

		SPMR::PushSprite(new Enemy({ 110,0,55,140 }, { 490.0f, 200.0f, 50.0f, 106.0f },
			Engine::Instance().GetRenderer(), TEMA::GetTexture("enemies"), 4, 1));
		SPMR::PushSprite(new VerticalEnemy({ 0,0,55,140 }, { 1600.0f, 600.0f, 50.0f, 106.0f },
			Engine::Instance().GetRenderer(), TEMA::GetTexture("enemies"), 4, 1));
		SPMR::PushSprite(new VerticalEnemy({ 0,0,55,140 }, { 3400.0f, 100.0f, 50.0f, 106.0f },
			Engine::Instance().GetRenderer(), TEMA::GetTexture("enemies"), 4, 1));
		SPMR::PushSprite(new VerticalEnemy({ 0,0,55,140 }, { 4200.0f, 30.0f, 50.0f, 106.0f },
			Engine::Instance().GetRenderer(), TEMA::GetTexture("enemies"), 4, 1));

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
	if (Engine::Instance().getLevel() == 3)
	{
		Engine::LoadLevel("Dat/Level3.txt");
		words[2]->SetText("Get those wigs back!");

		SPMR::PushSprite(new VerticalEnemy({ 0,0,55,140 }, { 900, 300, 50.0f, 106 }, Engine::Instance().GetRenderer(), TEMA::GetTexture("enemies")));
		SPMR::PushSprite(new VerticalEnemy({ 0,0,55,140 }, { 750.0f, 500.0f, 50.0f, 106.0f },
			Engine::Instance().GetRenderer(), TEMA::GetTexture("enemies"), 3, 1));
		SPMR::PushSprite(new Enemy({ 110,0,55,140 }, { 1700.0f, 200.0f, 50.0f, 106.0f },
			Engine::Instance().GetRenderer(), TEMA::GetTexture("enemies"), 3, 1));
		SPMR::PushSprite(new Enemy({ 110,0,55,140 }, { 2700.0f, 100.0f, 50.0f, 106.0f },
			Engine::Instance().GetRenderer(), TEMA::GetTexture("enemies"), 3, 1));
		SPMR::PushSprite(new Enemy({ 110,0,55,140 }, { 3900.0f, 500.0f, 50.0f, 106.0f },
			Engine::Instance().GetRenderer(), TEMA::GetTexture("enemies"), 3, 1));

		SPMR::PushSprite(new Wig({ 0,0,100,100 }, { 600.0f, 400.0f,50.0f,50.0f },
			Engine::Instance().GetRenderer(), TEMA::GetTexture("wig")));

		SPMR::PushSprite(new Energy({ 0,0,100,100 }, { 3700.0f, 100.0f,45.0f,45.0f },
			Engine::Instance().GetRenderer(), TEMA::GetTexture("lightning")));

		SPMR::PushSprite(new Health({ 0,0,256,256 }, { 4100.0f, 400.0f,50.0f,50.0f },
			Engine::Instance().GetRenderer(), TEMA::GetTexture("heart")));

		m_flag = new Sprite({ 0,0, 32, 64 }, { (32 * 137) , (32 * 20), 32, 64 }, Engine::Instance().GetRenderer(), TEMA::GetTexture("flag"));
	}
	if (Engine::Instance().getLevel() == 4)
	{
		Engine::LoadLevel("Dat/LevelBoss.txt");
		words[2]->SetText("...");

		// energy
		SPMR::PushSprite(new Energy({ 0,0,100,100 }, { (32.0f * 14.0f), (32.0f * 16.5f), 50.0f, 50.0f },
			Engine::Instance().GetRenderer(), TEMA::GetTexture("lightning")));
		SPMR::PushSprite(new Energy({ 0,0,100,100 }, { (32.0f * 11.0f), (32.0f * 1.5f), 50.0f, 50.0f },
			Engine::Instance().GetRenderer(), TEMA::GetTexture("lightning")));
		SPMR::PushSprite(new Energy({ 0,0,100,100 }, { (32.0f * 35.0f), (32.0f * 10.5f), 50.0f, 50.0f },
			Engine::Instance().GetRenderer(), TEMA::GetTexture("lightning")));
		SPMR::PushSprite(new Energy({ 0,0,256,256 }, { (32.0f * 55.0f), (32.0f * 9.5f), 50.0f, 50.0f },
			Engine::Instance().GetRenderer(), TEMA::GetTexture("lightning")));
		SPMR::PushSprite(new Energy({ 0,0,100,100 }, { (32.0f * 70.0f), (32.0f * 15.5f), 50.0f, 50.0f },
			Engine::Instance().GetRenderer(), TEMA::GetTexture("lightning")));
		SPMR::PushSprite(new Energy({ 0,0,100,100 }, { (32.0f * 94.0f), (32.0f * 8.5f), 50.0f, 50.0f },
			Engine::Instance().GetRenderer(), TEMA::GetTexture("lightning")));
		SPMR::PushSprite(new Energy({ 0,0,100,100 }, { (32.0f * 77.0f), (32.0f * 2.5f), 50.0f, 50.0f },
			Engine::Instance().GetRenderer(), TEMA::GetTexture("lightning")));
		SPMR::PushSprite(new Energy({ 0,0,100,100 }, { (32.0f * 122.0f), (32.0f * 9.5f), 50.0f, 50.0f },
			Engine::Instance().GetRenderer(), TEMA::GetTexture("lightning")));

		// health
		SPMR::PushSprite(new Health({ 0,0,256,256 }, { (32.0f * 8.0f), (32.0f * 44.5f), 50.0f, 50.0f },
			Engine::Instance().GetRenderer(), TEMA::GetTexture("heart")));
		SPMR::PushSprite(new Health({ 0,0,256,256 }, { (32.0f * 14.0f), (32.0f * 14.5f), 50.0f, 50.0f },
			Engine::Instance().GetRenderer(), TEMA::GetTexture("heart")));
		SPMR::PushSprite(new Health({ 0,0,256,256 }, { (32.0f * 69.0f), (32.0f * 20.5f), 50.0f, 50.0f },
			Engine::Instance().GetRenderer(), TEMA::GetTexture("heart")));
		SPMR::PushSprite(new Health({ 0,0,256,256 }, { (32.0f * 105.0f), (32.0f * 2.5f), 50.0f, 50.0f },
			Engine::Instance().GetRenderer(), TEMA::GetTexture("heart")));
		SPMR::PushSprite(new Health({ 0,0,256,256 }, { (32.0f * 108.0f), (32.0f * 18.5f), 50.0f, 50.0f },
			Engine::Instance().GetRenderer(), TEMA::GetTexture("heart")));

		SPMR::PushSprite(new Boss({ 220,0,55,140 }, { 4100.0f, 100.0f, 300, 636.0f },
			Engine::Instance().GetRenderer(), TEMA::GetTexture("enemies"), 4, 1));

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
	words[1]->SetText(to_string((int)(m_pPlayer->getParts())).c_str());

	m_pPlayer->Update(); // Change to player Update here.
	CheckCollision();
}


void GameState::CheckCollision()
{	
	for(int i = 0; i < SPMR::GetEnemies().size();i++)
	{
		m_pEnemy = SPMR::GetEnemies()[i];
		if (COMA::CircleAABBCheck(m_pPlayer->getCenter(), m_pPlayer->GetDstP()->w*.1, *(m_pEnemy->GetDstP())))
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

		if (Engine::Instance().getLevel() == 1) {
			timeToSwitchLevels = true;
			Engine::Instance().setLevel(2);
		}
		else if (Engine::Instance().getLevel() == 2 && m_pPlayer->getParts() >= 1) // currently at one for making testing quicker, change 5 later
		{
			timeToSwitchLevels = true;
			Engine::Instance().setLevel(3);
		}
		else if (Engine::Instance().getLevel() == 3 && m_pPlayer->getWigs() >= 10)
		{
			timeToSwitchLevels = true;
			Engine::Instance().setLevel(4);
		}

	}
	// don't fall in the pit and crash the game!
	if (m_pPlayer->GetY() > (HEIGHT - 128)) // idk where the code for catching kiki is rn... so the hard code kinda temp
	{
		m_pPlayer->GoBack();
	}
	if (m_pPlayer->getHealth() <= 0)
	{
		gameOver = true;
		m_pPlayer->setWigs(wigCount);
		m_pPlayer->setParts(partsCount);
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

	SPMR::Render();
	m_pauseBtn->Render();
	m_pReticle->Render();

	// ui stuff
	for (int i = 0; i < (m_pPlayer->getHealth()); i++)
		hpUI[i]->Render();
	for (int i = 0; i < (m_pPlayer->getEnergy()); i++)
		stungunUI[i]->Render();
	wigUI->Render();
	shipUI->Render();
	for (unsigned i = 0; i < 3; i++)
		words[i]->Render();

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
	SDL_ShowCursor(SDL_ENABLE);
	
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
	SDL_ShowCursor(SDL_ENABLE);
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
	SDL_SetRenderDrawBlendMode(Engine::Instance().GetRenderer(), SDL_BLENDMODE_BLEND);
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
	SDL_ShowCursor(SDL_DISABLE);
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