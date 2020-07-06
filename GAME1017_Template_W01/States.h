#pragma once
#ifndef _STATES_H_
#define _STATES_H_

#include <SDL.h>
#include "Button.h"
#include "PlatformPlayer.h"
#include "Enemy.h"
#include "Sprite.h"
#include "Pickup.h"
#include "StunGun.h"
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
	Button* m_pauseBtn;

	std::vector<Pickup*> m_pPickUpList;

	std::array<std::array<Tile*, COLS>, ROWS> m_level;
	std::vector<Sprite*> m_platforms;
	bool m_bgScrollX = false, m_bgScrollY = false;
	bool gameOver;

	Sprite* m_flag;

	Sprite* hpUI[5];
	Sprite* stungunUI[5];
	Sprite* wigUI;
	Sprite* shipUI;
	Label* words[2]; // 0 = wig counter, 1 = ship parts

public:
	GameState();
	void Update();
	void UpdateTiles(float scroll, bool x = false);
	void CheckCollision();
	void Render();
	void Enter();
	void Exit();
	void Resume();

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
	Label* words[4];
	Button* m_playBtn;
	Button* m_quitBtn;
	Button* m_controlsBtn;
	Sprite* instructions;
	Sprite* background;
	bool displayControls = false;
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
	Sprite* instructions;
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