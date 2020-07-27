#pragma once
#ifndef _EVENTMANAGER_H_
#define _EVENTMANAGER_H_

#include <SDL.h>

class EventManager
{
public:
	static void Init();
	static void HandleEvents();
	static char* getText();
	static bool KeyHeld(const SDL_Scancode c);
	static bool KeyPressed(const SDL_Scancode c);
	static bool KeyReleased(const SDL_Scancode c);
	static int LastKeyDown();
	static int LastKeyUp();
	static bool MouseHeld(const int b);
	static bool MousePressed(const int b);
	static bool MouseReleased(const int b);
	static void SetCursor(const SDL_SystemCursor& c);
	static SDL_Point& GetMousePos();
	static SDL_FPoint& GetMousePosF();
	static void Quit();
private:
	// Keyboard state containers.
	static const Uint8* s_keysCurr;
	static Uint8* s_keysLast;
	static int s_numKeys;
	// Mouse state containers.
	static int s_lastKeyDown;
	static char* s_text;
	static int s_lastKeyUp;
	static SDL_Point s_mousePos;
	static SDL_FPoint s_mousePosF;
	static Uint32 s_mouseCurr;
	static Uint32 s_mouseLast;
	static SDL_Cursor* s_cursor;
private: 
	EventManager() {}
};

typedef EventManager EVMA;

#endif