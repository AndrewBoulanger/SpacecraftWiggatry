#include "Engine.h"
#include "CollisionManager.h"
#include "DebugManager.h"
#include "EventManager.h"
#include "FontManager.h"
#include "SoundManager.h"
#include "StateManager.h"
#include "TextureManager.h"
#include "SpriteManager.h"
#include "PlatformPlayer.h"
#include <iostream>
#include <fstream>

#define FPS 60

using namespace std;

Engine::Engine():m_running(false){ cout << "Engine class constructed!" << endl; }

bool Engine::Init(const char* title, int xpos, int ypos, int width, int height, int flags)
{
	cout << "Initializing game..." << endl;
	// Attempt to initialize SDL.
	if (SDL_Init(SDL_INIT_EVERYTHING) == 0)
	{
		// Create the window.
		m_pWindow = SDL_CreateWindow(title, xpos, ypos, width, height, flags);
		if (m_pWindow != nullptr) // Window init success.
		{
			m_pRenderer = SDL_CreateRenderer(m_pWindow, -1, 0);
			if (m_pRenderer != nullptr) // Renderer init success.
			{
				EVMA::Init();
				SOMA::Init();
				TEMA::Init();
			}
			else return false; // Renderer init fail.
		}
		else return false; // Window init fail.
	}
	else return false; // SDL init fail.
	m_fps = (Uint32)round((1 / (double)FPS) * 1000); // Sets FPS in milliseconds and rounds.
	TEMA::RegisterTexture("Img/play.png", "play");
	TEMA::RegisterTexture("Img/exit.png", "exit");
	TEMA::RegisterTexture("Img/replay.png", "replay");
	TEMA::RegisterTexture("Img/resume.png", "resume");
	TEMA::RegisterTexture("Img/pause.png", "pause");
	TEMA::RegisterTexture("Img/Controls_Button.png", "control");
	TEMA::RegisterTexture("Img/controls.png", "controls");
	TEMA::RegisterTexture("Img/background.png", "bg");
	TEMA::RegisterTexture("Img/alienBeard.png", "enemy2");
	TEMA::RegisterTexture("Img/Enemies.png", "enemies");
	TEMA::RegisterTexture("Img/projectiles.png", "laser");

	TEMA::RegisterTexture("Img/KikiSprite2.0.png", "player");
	TEMA::RegisterTexture("Img/alienWhisker.png", "enemy");
	TEMA::RegisterTexture("Img/reticle.png", "reticle");
	TEMA::RegisterTexture("IMG/wig1.png", "wig");
	TEMA::RegisterTexture("IMG/shippart.png", "shippart");
	TEMA::RegisterTexture("Img/reticle.png", "hookshot");
	TEMA::RegisterTexture("Img/line.png", "line(temp)");

	TEMA::RegisterTexture("Img/hp.png", "heart");
	TEMA::RegisterTexture("Img/hp1.png", "heart1");
	TEMA::RegisterTexture("Img/hp2.png", "heart2");
	TEMA::RegisterTexture("Img/lightning.png", "lightning");

	TEMA::RegisterTexture("Img/Tiles2.png", "tiles");
	TEMA::RegisterTexture("Img/flag.png", "flag");

	SPMR::setPlayer( new PlatformPlayer({ 0,0,139,153 }, { 100.0f,600.0f,64.0f,96.0f },
		Engine::Instance().GetRenderer(), TEMA::GetTexture("player"), 0, 0, 2, 6)); 

	FOMA::RegisterFont("Img/font.ttf", "fontLarge", 150);
	FOMA::RegisterFont("Img/font.ttf", "font", 35);
	FOMA::RegisterFont("Img/font.ttf", "fontSmall", 20);
	SOMA::AllocateChannels(16);
	SOMA::Load("Aud/Poker Face.mp3", "PokerFace", SOUND_MUSIC);
	SOMA::Load("Aud/Wrecking Ball.mp3", "WreckingBall", SOUND_MUSIC);
	SOMA::Load("Aud/SweetVictory.mp3", "SweetVictory", SOUND_MUSIC);
	SOMA::Load("Aud/jump.wav", "jump", SOUND_SFX);
	SOMA::Load("Aud/dead.wav", "dead", SOUND_SFX);
	SOMA::Load("Aud/attack.wav", "slap", SOUND_SFX);
	SOMA::Load("Aud/laser.wav", "stun", SOUND_SFX);
	SOMA::Load("Aud/oof.mp3", "oof", SOUND_SFX);
	SOMA::Load("Aud/blub.mp3", "blub", SOUND_SFX);
	SOMA::Load("Aud/yahoo.wav", "yahoo", SOUND_SFX);
	SOMA::Load("Aud/wig_andrew.wav", "wig0", SOUND_SFX);
	SOMA::Load("Aud/wig_juan.wav", "wig1", SOUND_SFX);
	SOMA::Load("Aud/wig_pauleen.wav", "wig2", SOUND_SFX);
	SOMA::SetMusicVolume(getBgmVolume());
	SOMA::SetSoundVolume(getSfxVolume());
	STMA::ChangeState(new TitleState);

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
	
	m_running = true; // Everything is okay, start the engine.
	cout << "Engine Init success!" << endl;
	return true;
}

void Engine::Wake()
{
	m_start = SDL_GetTicks();
}

void Engine::Sleep()
{
	m_end = SDL_GetTicks();
	m_delta = m_end - m_start;
	if (m_delta < m_fps) // Engine has to sleep.
		SDL_Delay(m_fps - m_delta);
}

void Engine::HandleEvents()
{
	EVMA::HandleEvents();
}

void Engine::Update()
{
	if (EVMA::KeyReleased(SDL_SCANCODE_H))
		DEMA::TogleDebug();
	STMA::Update();
}

void Engine::Render() 
{
	SDL_SetRenderDrawColor(m_pRenderer, 0, 0, 0, 255);
	SDL_RenderClear(m_pRenderer); // Clear the screen with the draw color.
	// Draw anew.
	STMA::Render();
	SDL_RenderPresent(m_pRenderer);

}

void Engine::Clean()
{
	cout << "Cleaning game." << endl;
	SDL_DestroyRenderer(m_pRenderer);
	SDL_DestroyWindow(m_pWindow);
	DEMA::Quit();
	EVMA::Quit();
	FOMA::Quit();
	SOMA::Quit();
	STMA::Quit();
	TEMA::Quit();
	IMG_Quit();
	SDL_Quit();
}

int Engine::Run()
{
	if (m_running) // What does this do and what can it prevent?
		return -1; 
	if (Init("SPACECRAFT: Wiggatry", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, 0) == false)
		return 1;
	while (m_running) // Main engine loop.
	{
		Wake();
		HandleEvents();
		Update();
		Render();
		if (m_running)
			Sleep();
	}
	Clean();
	return 0;
}

Engine& Engine::Instance()
{
	static Engine instance; // C++11 will prevent this line from running more than once. Magic statics.
	return instance;
}

void Engine::LoadLevel(std::string path)
{
	std::ifstream inFile(path);
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
				SPMR::PushSprite(m_level[row][col], background);
			}
		}
	}
	inFile.close();
}


SDL_Renderer* Engine::GetRenderer() { return m_pRenderer; }
bool& Engine::Running() { return m_running; }

std::array<std::array<Tile*, COLS>, ROWS> Engine::GetLevel() { return m_level; }

std::array<std::array<Tile*, COLS>, ROWS> Engine::m_level;
std::map<char, Tile*> Engine::m_tiles;