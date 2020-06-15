#pragma once
#ifndef _STATES_H_
#define _STATES_H_

#include <SDL.h>
#include "Button.h"
#include "PlatformPlayer.h"
#include "Enemy.h"
#include "Sprite.h"
#include "Pickup.h"
#include <vector>
#include <map>

class State // This is the abstract base class for all specific states.
{
protected:
	State() {}
public:
	virtual void Update() = 0; // Having at least one 'pure virtual' method like this, makes a class abtract.
	virtual void Render();     // Meaning we cannot create objects of the class.
	virtual void Enter() = 0;  // Virtual keyword means we can override in derived class.
	virtual void Exit() = 0;
	virtual void Resume();
};

class GameState : public State
{
private:
	PlatformPlayer* m_pPlayer;
	Enemy* m_pEnemy;
	Sprite* m_pReticle;

	std::vector<Pickup*> m_pPickUpList;
	std::array<std::array<Tile*, COLS>, ROWS> m_level;
	std::map<char, Tile*> m_tiles;

	Sprite* hpUI[5];

public:
	GameState();
	void Update();
	void CheckCollision();
	void Render();
	void Enter();
	void Exit();
	void Resume();
	std::array < std::array<Tile*, COLS>, ROWS>& GetLevel() { return m_level; }
	static GameState& Instance();
};

class TitleState : public State
{
public:
	TitleState();
	void Update();
	void Render();
	void Enter();
	void Exit();
private:
	Label* words[3];
	Button* m_playBtn;
	Button* m_quitBtn;
};

class PauseState : public State
{
public:
	PauseState();
	void Update();
	void Render();
	void Enter();
	void Exit();
private:
	Button* m_resumeBtn;
	Button* m_menuBtn;
};

class DeadState : public State
{
public:
	DeadState();
	void Update();
	void Render();
	void Enter();
	void Exit();
private:
	Label* words;
	Button* m_playBtn;
	Button* m_quitBtn;
};

#endif