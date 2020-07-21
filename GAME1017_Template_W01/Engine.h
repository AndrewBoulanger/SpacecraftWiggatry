#pragma once
#ifndef _ENGINE_H_
#define _ENGINE_H_
#define ROWS 24
#define COLS 140

#define FPS 60

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>
#include <array>
#include <map>
#include "Label.h"
#include "Tile.h"

#define WIDTH 1024
#define HEIGHT 768

// Do not include any macros, initialized properties or full function definitions in this header.

class Engine
{
private: // Private properties.
	bool m_running; // Loop control flag.
	Uint32 m_start, m_end, m_delta, m_fps; // Fixed timestep variables.
	SDL_Window* m_pWindow; // This represents the SDL window.
	SDL_Renderer* m_pRenderer; // This represents the buffer to draw to.
	// Example-specific properties.

	static std::map<char, Tile*> m_tiles;
	static std::array<std::array<Tile*, COLS>, ROWS> m_level;

	int currentLevel = 1;

	int bgmVolume = 30; // idk how to get volume value from sdl...
	int sfxVolume = 30;

private: // Private methods.
	Engine();
	bool Init(const char* title, int xpos, int ypos, int width, int height, int flags);
	void Wake();
	void Sleep();
	void HandleEvents();
	void Update();
	void Render();
	void Clean();
public: // Public methods.
	int Run();
	static Engine& Instance(); // This static method creates the static instance that can be accessed 'globally'
	SDL_Renderer* GetRenderer();
	bool& Running();

	static std::array<std::array<Tile*, COLS>, ROWS> GetLevel();
	static void LoadLevel(std::string path);
	int getLevel() { return currentLevel; };
	void setLevel(int l) { currentLevel = l; };

	int getBgmVolume() { return bgmVolume; }
	void setBgmVolume(int v) { bgmVolume = v; }
	int getSfxVolume() { return sfxVolume; }
	void setSfxVolume(int v) { sfxVolume = v; }
	
};

#endif