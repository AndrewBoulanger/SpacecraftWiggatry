#pragma once
#ifndef _STATES_H_
#define _STATES_H_

#include <SDL.h>
#include "Button.h"
#include "UIElement.h"
#include "PlatformPlayer.h"
#include "Enemy.h"
#include "Sprite.h"
#include "Pickup.h"
#include "StunGun.h"
#include "tinyxml2.h"
#include <vector>
#include <string>
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
	int wigCount, partsCount;

	//std::vector<Pickup*> m_pPickUpList;

	bool gameOver;
	bool timeToSwitchLevels;

	Sprite* m_flag;
	Enemy* m_pBoss;

	Sprite* hpUI[5];
	Sprite* hpEnemyUI[10];
	Sprite* stungunUI[5];
	Sprite* wigUI;
	Sprite* shipUI;
	Label* words[3]; // 0 = wig counter, 1 = ship parts, 2 = objective

public:
	GameState();
	void Update();
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
	Label* words[6];
	Button* m_playBtn;
	Button* m_quitBtn;
	Button* m_controlsBtn;
	Sprite* instructions;
	Sprite* background;
	bool displayControls = false;

	// for name input
	std::string playerName = "";
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
	UIElement* bgmVolume;
	UIElement* sfxVolume;

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

class EndState : public State
{
public:
	EndState();
	void Update();
	void Render();
	void Enter();
	void Exit();
	void Load();
	void Save();
private:
	PlatformPlayer* m_pPlayer;
	Label* words[27];
	Button* m_playBtn;
	Button* m_quitBtn;

	std::vector<std::string> playerName;
	std::vector<int> wigScore;
	std::vector<int> shipScore;
	std::vector<int> totalScore;
};

#endif