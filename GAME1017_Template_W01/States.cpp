#include "States.h"
#include "CollisionManager.h"
#include "EventManager.h"
#include "SoundManager.h"
#include "StateManager.h" // Make sure this is NOT in "States.h" or circular reference.
#include "TextureManager.h"
#include "MathManager.h"
#include "Engine.h"
#include "Button.h"
#include "Enemy.h"
#include <iostream>
#include <fstream>


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
	m_pPlayer = new PlatformPlayer({ 0,0,400,152 }, { 400.0f,600.0f,96.0f,96.0f }, 
								   Engine::Instance().GetRenderer(), TEMA::GetTexture("player"));
	m_pEnemy = new Enemy({ 0,0,400,140 }, {850.0f, 400.0f, 50.0f, 120.0f}, 
									Engine::Instance().GetRenderer(), TEMA::GetTexture("enemy"), 10, 10);
	for (int i = 0; i < (5); i++)
		hpUI[i] = new Sprite({ 0,0, 256,256 }, { (float)(35*i),0, 35,35 }, Engine::Instance().GetRenderer(), TEMA::GetTexture("heart"));
	m_pReticle = new Sprite({ 0,0, 36,36 }, { 0,0, 25,25 }, Engine::Instance().GetRenderer(), TEMA::GetTexture("reticle"));

	std::ifstream inFile("Dat/Tiledata.txt");
	if (inFile.is_open())
	{ // Create map of Tile prototypes.
		char key;
		int x, y;
		bool o, h;
		while (!inFile.eof())
		{
			inFile >> key >> x >> y >> o >> h;
			m_tiles.emplace(key, new Tile({ x * 32, y * 32, 32, 32 }, { 0,0,32,32 }, Engine::Instance().GetRenderer(), TEMA::GetTexture("tiles"), o, h));
		}
	}
	inFile.close();
	inFile.open("Dat/Level1.txt");
	if (inFile.is_open())
	{ // Build the level from Tile prototypes.
		char key;
		for (int row = 0; row < ROWS; row++)
		{
			for (int col = 0; col < COLS; col++)
			{
				inFile >> key;
				m_level[row][col] = m_tiles[key]->Clone(); // Prototype design pattern used.
				m_level[row][col]->GetDstP()->x = (float)(32 * col);
				m_level[row][col]->GetDstP()->y = (float)(32 * row);
			}
		}
	}
	inFile.close();

	m_pPickUpList.push_back(new Wig({ 0,0,100,100 }, { 600.0f, 400.0f,50.0f,50.0f },
					Engine::Instance().GetRenderer(), TEMA::GetTexture("wig")));

	SOMA::Load("Aud/jump.wav", "jump", SOUND_SFX);
	
	SOMA::PlayMusic("PokerFace");
	
}

void GameState::Update()
{

	m_pReticle->SetPos(EVMA::GetMousePos());
	m_pPlayer->Update();
	//m_pEnemy->Update();
	CheckCollision();
	if (EVMA::KeyPressed(SDL_SCANCODE_P))
	{
		STMA::PushState(new PauseState);
	}

	for (int i = 0; i < m_pPickUpList.size(); i++)
		if(m_pPickUpList[i] != nullptr)m_pPickUpList[i]->Update();

}

void GameState::CheckCollision()
{	// previous platforms cc
	//for (int i = 0; i < NUMPLATFORMS; i++) // For each platform.
	//{
	//	if (COMA::AABBCheck(*m_pPlayer->GetDstP(), *m_pPlatforms[i]))
	//	{
	//		if (m_pPlayer->GetDstP()->x + m_pPlayer->GetDstP()->w - m_pPlayer->GetVelX() <= m_pPlatforms[i]->x)
	//		{ // Collision from left.
	//			m_pPlayer->StopX(); // Stop the player from moving horizontally.
	//			m_pPlayer->SetX(m_pPlatforms[i]->x - m_pPlayer->GetDstP()->w);
	//		}
	//		else if (m_pPlayer->GetDstP()->x - (float)m_pPlayer->GetVelX() >= m_pPlatforms[i]->x + m_pPlatforms[i]->w)
	//		{ // Colliding right side of platform.
	//			m_pPlayer->StopX();
	//			m_pPlayer->SetX(m_pPlatforms[i]->x + m_pPlatforms[i]->w);
	//		}
	//		else if (m_pPlayer->GetDstP()->y + m_pPlayer->GetDstP()->h - (float)m_pPlayer->GetVelY() <= m_pPlatforms[i]->y)
	//		{ // Colliding top side of platform.
	//			m_pPlayer->SetGrounded(true);
	//			m_pPlayer->StopY();
	//			m_pPlayer->SetY(m_pPlatforms[i]->y - m_pPlayer->GetDstP()->h - 1);
	//		}
	//		else if (m_pPlayer->GetDstP()->y - (float)m_pPlayer->GetVelY() >= m_pPlatforms[i]->y + m_pPlatforms[i]->h)
	//		{ // Colliding bottom side of platform.
	//			m_pPlayer->StopY();
	//			m_pPlayer->SetY(m_pPlatforms[i]->y + m_pPlatforms[i]->h);
	//		}
	//	}
	//}
	if (COMA::AABBCheck(*m_pPlayer->GetDstP(), *m_pEnemy->GetDstP()))
	{
		if (m_pPlayer->GetDstP()->x - (float)m_pPlayer->GetVelX() >= m_pEnemy->GetDstP()->x + m_pEnemy->GetDstP()->w)
		{ // Colliding right side of platform.
			m_pPlayer->StopX();
			m_pPlayer->KnockLeft(-10); //knock the player to the right
		}
		else
		{
			m_pPlayer->Stop();
			m_pPlayer->KnockLeft(10);
		}
		m_pPlayer->takeDamage(m_pEnemy->getBaseDamage());
	}
	for (int i = 0; i < m_pPickUpList.size(); i++)
	{
		if (COMA::CircleCircleCheck(m_pPlayer->getCenter(), m_pPickUpList[i]->getCenter(), 40, 50))
		{
			switch (m_pPickUpList[i]->getType())
			{
			case WIG:
				m_pPlayer->add1Wig();
				break;
			case SHIP_PART:
				m_pPlayer->add1ShipPart();
				break;
			default:
				break;
			}
			delete m_pPickUpList[i];
			m_pPickUpList[i] = nullptr;
			std::cout << "collected\n";
			m_pPickUpList.erase(m_pPickUpList.begin()+i);
			m_pPickUpList.shrink_to_fit();
		}
	}

}

void GameState::Render()
{
	SDL_SetRenderDrawColor(Engine::Instance().GetRenderer(), 64, 128, 255, 255);
	SDL_RenderClear(Engine::Instance().GetRenderer());
	SDL_RenderCopy(Engine::Instance().GetRenderer(), TextureManager::GetTexture("bg"), 0, 0);
	for (int row = 0; row < ROWS; row++)
	{
		for (int col = 0; col < COLS; col++)
		{
			m_level[row][col]->Render();
		}
	}
	//draw the enemy
	m_pEnemy->Render();
	// Draw the player.
	if(m_pPlayer->getICoolDown()%10 < 5)
		m_pPlayer->Render();
	
	// Draw the platforms.
	SDL_SetRenderDrawColor(Engine::Instance().GetRenderer(), 70, 192, 0, 255);

	m_pReticle->Render();

	for (int i = 0; i < m_pPickUpList.size(); i++)
		m_pPickUpList[i]->Render();

	for (int i = 0; i < (m_pPlayer->getHealth()/10); i++)
		hpUI[i]->Render();


	// If GameState != current state.
	if (dynamic_cast<GameState*>(STMA::GetStates().back()))
		State::Render();

}

void GameState::Exit()
{
	delete m_pPlayer;
	delete m_pEnemy;
	delete m_pReticle;
	for (int i = 0; i < m_pPickUpList.size(); i++)
	{
		delete m_pPickUpList[i];
		m_pPickUpList[i] = nullptr;
	}
	m_pPickUpList.clear();
	m_pPickUpList.shrink_to_fit();
}

void GameState::Resume() { }
// End GameState.

// Begin TitleState.
TitleState::TitleState() {}

void TitleState::Enter()
{
	m_playBtn = new PlayButton({ 0,0,400,100 }, { 312.0f,400.0f,400.0f,100.0f }, Engine::Instance().GetRenderer(), TEMA::GetTexture("play"));
	m_quitBtn = new QuitButton({ 0,0,400,100 }, { 312.0f,520.0f,400.0f,100.0f }, Engine::Instance().GetRenderer(), TEMA::GetTexture("exit"));
	words[0] = new Label("font", 180, 110, "SPACECRAFT", { 188,7,208,0 });
	words[1] = new Label("font", 260, 200, "Wiggatry", { 255,255,255,0 });
	words[2] = new Label("font", 0, 670, "ETTG", { 255,0,180,0 });
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
	for (int i = 0; i < 3; i++)
		words[i]->Render();
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
	words = new Label("font", 220, 110, "GAME OVER", { 255,255,255,0 });
	m_playBtn = new PlayButton({ 0,0,400,100 }, { 312.0f,400.0f,400.0f,100.0f }, Engine::Instance().GetRenderer(), TEMA::GetTexture("replay"));
	m_quitBtn = new QuitButton({ 0,0,400,100 }, { 312.0f,520.0f,400.0f,100.0f }, Engine::Instance().GetRenderer(), TEMA::GetTexture("exit"));
	SOMA::Load("Aud/power.wav", "beep", SOUND_SFX);
	SOMA::PlayMusic("WreckingBall");
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