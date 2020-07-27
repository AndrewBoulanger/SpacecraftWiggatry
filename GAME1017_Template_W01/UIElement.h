#pragma once
#ifndef _UIELEMENT_H_
#define _UIELEMENT_H_

#include "Label.h"
#include "SoundManager.h"
#include <algorithm>
#include <SDL.h>

enum uiState { STATE_UP, STATE_OVER, STATE_DOWN };

class UIElement // Abstract base class.
{
public: // Whatever we need to access through the base class pointer has to be public.
	virtual ~UIElement() {  } // For delete calls in this example.
	virtual bool Update() = 0;
	virtual void Render() = 0;

protected:
	UIElement(SDL_Rect d, SDL_Renderer* r, SDL_Color c);
	Uint8 ClampRGB(int c) { return (Uint8)(std::min(std::max(c, 0), 255)); }

protected:
	SDL_Renderer* m_pRend;
	SDL_Rect m_dst;
	SDL_Color m_color;
	uiState m_state;
};

class UIButton : public UIElement // Also abstract;
{
public:
	bool Update();
	void Render();

protected:
	UIButton(SDL_Rect d, SDL_Renderer* r, SDL_Color c, const char* name);
	~UIButton(); // Why can destructor be protected and not public?
	virtual void Execute() = 0; // Makes it abstract.
	void SetState(const uiState& s, const int y);

private:
	SDL_Rect m_new;
	Label* m_pName;
};

class UISlider : public UIElement
{
public:
	UISlider(SDL_Rect d, SDL_Renderer* r, SDL_Color col, SDL_Color colText, const char* name, int startVal = 100, int minVal = 0, int maxVal = 100);
	bool Update();
	void Render();
	int Value() { return m_val; }
	void SetValue(int i);

private:
	~UISlider(); // Again. Why can destructor be private in this example?

private:
	int m_val, m_minVal, m_maxVal;
	SDL_Rect m_handle;
	Label* m_pName, * m_pVal, * m_pMin, * m_pMax;
};

#endif