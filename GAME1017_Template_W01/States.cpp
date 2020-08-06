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
#include "SpriteManager.h"
#include "Utilities.h"
#include "VerticalEnemy.h"
#include "Boss.h"
#include <iostream>
#include <vector>
#include <fstream>
#include <string>


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
	Engine::Instance().setLevel(4);
	std::cout << "Entering GameState..." << std::endl;
	std::cout << "This is the name of our beautiful player: " << Engine::Instance().getName() << std::endl;
	SDL_ShowCursor(SDL_DISABLE); // we have a reticle so...
	m_pPlayer = SPMR::getPlayer();
	m_pBoss = new Boss({ 220,0,55,140 }, { 4100.0f, 100.0f, 300, 636.0f },
		Engine::Instance().GetRenderer(), TEMA::GetTexture("enemies"), 4, 1); // object to get boss health

	m_pauseBtn = new PauseButton({ 0,0,86,78 }, { 1005.0f,0.0f,21.5f,19.5f }, Engine::Instance().GetRenderer(), TEMA::GetTexture("pause"));
	m_pReticle = new Sprite({ 0,0, 36,36 }, { 0,0, 25,25 }, Engine::Instance().GetRenderer(), TEMA::GetTexture("reticle"));

	wigCount = m_pPlayer->getWigs();
	partsCount = m_pPlayer->getParts();

	// ui stuff
	for (int i = 0; i < 5; i++)
		hpUI[i] = new Sprite({ 0,0, 256,256 }, { (float)(35*i),0, 35,35 }, Engine::Instance().GetRenderer(), TEMA::GetTexture("heart"));
	for (int i = 0; i < 5; i++)
		stungunUI[i] = new Sprite({ 0,0, 29,35 }, { (float)(35 * i),36, 29,32 }, Engine::Instance().GetRenderer(), TEMA::GetTexture("lightning"));
	wigUI = new Sprite({ 0,0, 100,100 }, { (float)(185),0, 35,35 }, Engine::Instance().GetRenderer(), TEMA::GetTexture("wig"));
	shipUI = new Sprite({ 0,0, 74, 75 }, { (float)(250),-3, 35,33 }, Engine::Instance().GetRenderer(), TEMA::GetTexture("shippart"));
	words[0] = new Label("font", 225, 4, to_string((int)(m_pPlayer->getWigs())).c_str(), { 255,255,255,0 });
	words[1] = new Label("font", 289, 4, to_string((int)(m_pPlayer->getParts())).c_str(), { 255,255,255,0 });
	words[2] = new Label("font", 15, 720, "objective updated base on level", { 255,255,255,0 });

	gameOver = false; 
	timeToSwitchLevels = false;

	m_pPlayer->SetPos({ (int)100.0f,(int)600.0f });

	SOMA::PlayMusic("PokerFace");

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

		SPMR::PushSprite(m_pBoss); 

		m_flag = new Sprite({ 0,0, 32, 64 }, { (32 * 137) , (32 * 20), 32, 64 }, Engine::Instance().GetRenderer(), TEMA::GetTexture("flag"));

		SOMA::StopMusic(15);
	}

	SPMR::PushSprite(m_flag, Regular);

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

	if (COMA::AABBCheck(*m_pPlayer->GetDstP(), *m_flag->GetDstP())) 
	{
		if (Engine::Instance().getLevel() == 1) {
			timeToSwitchLevels = true;
			Engine::Instance().setLevel(2);
		}
		else if (Engine::Instance().getLevel() == 2 && m_pPlayer->getParts() >= 1) // for testing, get 1 at least
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
	if (m_pPlayer->GetY() > (HEIGHT-10)) // idk where the code for catching kiki is rn... so the hard code kinda temp
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
	words[3] = new Label("font", 200, 360, "Enter your name: ", { 255,255,255,0 });
	words[4] = new Label("font", 500, 360, "KIKI", { 255,255,255,0 }); // user inputed name [4]
	words[5] = new Label("font", 370, 600, "Press ENTER to return", { 255,255,255,0 });
	SOMA::Load("Aud/power.wav", "beep", SOUND_SFX);
}

void TitleState::Update()
{
	if (EVMA::LastKeyDown() == SDLK_BACKSPACE && playerName.length() > 0)
	{
		//lop off character
		playerName.pop_back();
		std::cout << "back";
		//inputText += EVMA::getText();
	}
	// copy
	if (EVMA::LastKeyDown() == SDLK_c && SDL_GetModState() & KMOD_CTRL)
	{
		SDL_SetClipboardText(playerName.c_str());
	}
	// and paste
	if (EVMA::LastKeyDown() == SDLK_v && SDL_GetModState() & KMOD_CTRL)
	{
		playerName = SDL_GetClipboardText();
	}
	// RIPPPPPP because I couldn't get the other method to work OMGGGG GUYYYYSSSSSSSS I'M SORRY FOR THE BLOCK HERE
	if (playerName.size() < 8)
	{
		if (EVMA::KeyPressed(SDL_SCANCODE_SPACE)) playerName += ' ';
		if (EVMA::KeyPressed(SDL_SCANCODE_A)) playerName += 'A';
		if (EVMA::KeyPressed(SDL_SCANCODE_B)) playerName += 'B';
		if (EVMA::KeyPressed(SDL_SCANCODE_C)) playerName += 'C';
		if (EVMA::KeyPressed(SDL_SCANCODE_D)) playerName += 'D';
		if (EVMA::KeyPressed(SDL_SCANCODE_E)) playerName += 'E';
		if (EVMA::KeyPressed(SDL_SCANCODE_F)) playerName += 'F';
		if (EVMA::KeyPressed(SDL_SCANCODE_G)) playerName += 'G';
		if (EVMA::KeyPressed(SDL_SCANCODE_H)) playerName += 'H';
		if (EVMA::KeyPressed(SDL_SCANCODE_I)) playerName += 'I';
		if (EVMA::KeyPressed(SDL_SCANCODE_J)) playerName += 'J';
		if (EVMA::KeyPressed(SDL_SCANCODE_K)) playerName += 'K';
		if (EVMA::KeyPressed(SDL_SCANCODE_L)) playerName += 'L';
		if (EVMA::KeyPressed(SDL_SCANCODE_M)) playerName += 'M';
		if (EVMA::KeyPressed(SDL_SCANCODE_N)) playerName += 'N';
		if (EVMA::KeyPressed(SDL_SCANCODE_O)) playerName += 'O';
		if (EVMA::KeyPressed(SDL_SCANCODE_P)) playerName += 'P';
		if (EVMA::KeyPressed(SDL_SCANCODE_Q)) playerName += 'Q';
		if (EVMA::KeyPressed(SDL_SCANCODE_R)) playerName += 'R';
		if (EVMA::KeyPressed(SDL_SCANCODE_S)) playerName += 'S';
		if (EVMA::KeyPressed(SDL_SCANCODE_T)) playerName += 'T';
		if (EVMA::KeyPressed(SDL_SCANCODE_U)) playerName += 'U';
		if (EVMA::KeyPressed(SDL_SCANCODE_V)) playerName += 'V';
		if (EVMA::KeyPressed(SDL_SCANCODE_W)) playerName += 'W';
		if (EVMA::KeyPressed(SDL_SCANCODE_X)) playerName += 'X';
		if (EVMA::KeyPressed(SDL_SCANCODE_Y)) playerName += 'Y';
		if (EVMA::KeyPressed(SDL_SCANCODE_Z)) playerName += 'Z';
	}
	if (playerName != "")
	{
		words[4]->SetText(playerName.c_str());
	}
	else // empty
	{
		words[4]->SetText(" ");
	}


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
	else
	{
		if (EVMA::KeyPressed(SDL_SCANCODE_RETURN))
		{
			displayControls = false;
		}
	}
}

void TitleState::Render()
{
	SDL_SetRenderDrawColor(Engine::Instance().GetRenderer(), 255, 51, 255, 0);
	SDL_RenderClear(Engine::Instance().GetRenderer());
	background->Render();
	for (int i = 0; i < 5; i++)
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
		words[6]->Render();
	}

	State::Render();
}

void TitleState::Exit()
{
	std::cout << "Exiting TitleState..." << std::endl;
	if (playerName == "") // if no name set, make it kiki
	{
		playerName = "KIKI";
	}
	Engine::Instance().setName(playerName);
}

// End TitleState.

// Pause StateStuff
PauseState::PauseState() {}

void PauseState::Enter()
{
	std::cout << "Entering Pause...\n";
	SDL_ShowCursor(SDL_ENABLE);
	m_resumeBtn = new ResumeButton({ 0,0,200,80 }, { 415.0f,550.0f,200.0f,80.0f }, Engine::Instance().GetRenderer(), TEMA::GetTexture("resume"));//1024
	instructions = new Sprite({ 0,0,525,350 }, { 260.5f,140.0f,525.0f,350.0f },Engine::Instance().GetRenderer(), TEMA::GetTexture("controls"));
	bgmVolume = new UISlider({ 240, 490, 256, 32 }, Engine::Instance().GetRenderer(), { 255, 0, 255, 255 }, { 255, 255, 255, 255 }, "Music", Engine::Instance().getBgmVolume(), 0, 128);
	sfxVolume = new UISlider({ 560, 490, 256, 32 }, Engine::Instance().GetRenderer(), { 255, 0, 255, 255 }, { 255, 255, 255, 255 }, "SFX", Engine::Instance().getSfxVolume(), 0, 128);
}

void PauseState::Update()
{
	if (m_resumeBtn->ButtonUpdate() == 1)
		return;
	bgmVolume->Update();
	sfxVolume->Update();
	SOMA::SetMusicVolume(static_cast<UISlider*>(bgmVolume)->Value());
	SOMA::SetSoundVolume(static_cast<UISlider*>(sfxVolume)->Value());
	Engine::Instance().setBgmVolume(static_cast<UISlider*>(bgmVolume)->Value());
	Engine::Instance().setSfxVolume(static_cast<UISlider*>(sfxVolume)->Value());
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
	bgmVolume->Render();
	sfxVolume->Render();
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
	std::cout << "Exiting EndState..." << std::endl;
}
// End DeadState.

// Begin EndState.
EndState::EndState() {}

void EndState::Enter()
{
	std::cout << "Entering EndState...\n";
	EndState::Load();
	m_pPlayer = SPMR::getPlayer();
	words[0] = new Label("fontLarge", 200, 30, "WIGTASTIC!", { 255,0,255,0 });
	words[1] = new Label("fontSmall", 300, 150, "You managed to keep your job in the wig business!", { 255,200,220,0 });
	words[2] = new Label("font", 425, 215, "HIGHSCORES", { 255,0,255,0 });
	words[3] = new Label("fontSmall", 200, 300, "NAME:", { 255,255,255,0 });
	words[4] = new Label("fontSmall", 400, 300, "WIGS:", { 255,255,255,0 });
	words[5] = new Label("fontSmall", 550, 300, "SHIP PARTS:", { 255,255,255,0 });
	words[6] = new Label("fontSmall", 700, 300, "TOTAL SCORE:", { 255,255,255,0 });

	int shiftdown = 350;
	string name = Engine::Instance().getName();
	if (name == "")
		name = "some kiki";
	int wig = m_pPlayer->getWigs();
	int ship = m_pPlayer->getParts();
	int total = 1800;
	int pos = 5;
	//total = (m_pPlayer->getWigs() * 100) + (m_pPlayer->getParts() * 125);

	cout << totalScore[3];
	for (int i = 4; i >= 0; i--) // load the score information here! i: name, i+1: wig count, i+2: ship part count, i+3: total score(calulate and add)
	{
		if (total > totalScore[i])
		{
			pos--;
		}
	}
	if (pos < 5)
	{
		playerName.insert(playerName.begin() + pos, name);
		wigScore.insert(wigScore.begin() + pos, wig);
		shipScore.insert(shipScore.begin() + pos, ship);
		totalScore.insert(totalScore.begin() + pos, total);
	}

	// load file over this, for now, I will tempy define then variables

	int counter = 0;
	for (unsigned int i = 7; i < 27; i+=4) // load the score information here! i: name, i+1: wig count, i+2: ship part count, i+3: total score(calulate and add)
	{
		// get next file input and redefine variables each iteration here
		name = playerName[counter];
		words[i] = new Label("fontSmall", 200, shiftdown, name.c_str(), { 255,255,255,0 });
		words[i+1] = new Label("fontSmall", 400, shiftdown, to_string((int)(wigScore[counter])).c_str(), { 255,255,220,0 });
		words[i+2] = new Label("fontSmall", 550, shiftdown, to_string((int)(shipScore[counter])).c_str(), { 255,255,255,0 });
		words[i+3] = new Label("fontSmall", 700, shiftdown, to_string((int)(totalScore[counter])).c_str(), { 255,255,255,0 });

		shiftdown += 40; // to next line
		counter++;
	}


	m_playBtn = new PlayButton({ 0,0,400,100 }, { 70.0f,600.0f,400.0f,100.0f }, Engine::Instance().GetRenderer(), TEMA::GetTexture("replay"));
	m_quitBtn = new QuitButton({ 0,0,400,100 }, { 540.0f,600.0f,400.0f,100.0f }, Engine::Instance().GetRenderer(), TEMA::GetTexture("exit"));

	SOMA::PlayMusic("WreckingBall"); // maybe change to victory music? any recommendations?
	EndState::Save();
}

void EndState::Update()
{
	if (m_playBtn->ButtonUpdate() == 1)
		return;
	if (m_quitBtn->ButtonUpdate() == 1)
		return;
}

void EndState::Render()
{
	SDL_SetRenderDrawColor(Engine::Instance().GetRenderer(), 186, 13, 220, 0);
	SDL_RenderClear(Engine::Instance().GetRenderer());

	for (unsigned i = 0; i < 27; i++)
		words[i]->Render();

	m_playBtn->Render();
	m_quitBtn->Render();

	State::Render();
}

void EndState::Exit()
{
	std::cout << "Exiting EndState..." << std::endl;
}
void EndState::Load()
{
	cout << "Loading...";
	tinyxml2::XMLDocument xmlDoc;
	xmlDoc.LoadFile("Dat/HighScores.xml");
	tinyxml2::XMLElement* pRoot = xmlDoc.FirstChildElement();
	tinyxml2::XMLElement* pElement = pRoot->FirstChildElement();

	const char* n; // buffers
	string sName;
	int w, s, t;

	for (unsigned int i = 0; i < 5; i++)
	{
		if (strcmp(pElement->Value(), "HighScore") == 0)
		{
			pElement->QueryStringAttribute("name", &n); // "Gets" what'stored in name
			pElement->QueryIntAttribute("wig", &w);
			pElement->QueryIntAttribute("ship", &s);
			pElement->QueryIntAttribute("total", &t);
			sName = n; // trying to convert here
			playerName.push_back(sName);
			wigScore.push_back(w);
			shipScore.push_back(s);
			totalScore.push_back(t);
		}
		pElement = pElement->NextSiblingElement();
	}
	cout << "Load complete!\n";
}
void EndState::Save()
{
	cout << "Saving...";
	const char* name;
	tinyxml2::XMLDocument xmlDoc;

	tinyxml2::XMLNode* pRoot = xmlDoc.NewElement("Root");
	xmlDoc.InsertEndChild(pRoot);

	tinyxml2::XMLElement* pElement;

	for (int i = 0; i < 5; i++)
	{
		name = playerName[i].c_str();
		pElement = xmlDoc.NewElement("HighScore");
		pElement->SetAttribute("name", name);
		pElement->SetAttribute("wig", wigScore[i]);
		pElement->SetAttribute("ship", shipScore[i]);
		pElement->SetAttribute("total", totalScore[i]);
		pRoot->InsertEndChild(pElement);
	}
	xmlDoc.SaveFile("Dat/HighScoresTesting.xml"); //
	cout << "Save complete!\n";
}
// End DeadState.