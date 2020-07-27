#include "EventManager.h"
#include "Engine.h"
#include <cstring>
#include <iostream>

void EventManager::Init()
{
	s_keysCurr = SDL_GetKeyboardState(&s_numKeys);
	s_keysLast = new Uint8[s_numKeys];
	std::memcpy(s_keysLast, s_keysCurr, s_numKeys);
	s_mouseCurr = SDL_GetMouseState(&s_mousePos.x, &s_mousePos.y);
	s_mouseLast = s_mouseCurr;
	SDL_StartTextInput();
	std::cout << "EventManager init done!" << std::endl;
}

void EventManager::HandleEvents()
{
	SDL_Event event;
	
	std::memcpy(s_keysLast, s_keysCurr, s_numKeys);
	s_mouseLast = s_mouseCurr;
	s_lastKeyDown = s_lastKeyUp = -1;

	while (SDL_PollEvent(&event)) // Pump events invoked.
	{
		switch (event.type) // Parse some global events.
		{
			case SDL_QUIT: // User pressed window's 'x' button.
				Engine::Instance().Running() = false;
				break;
			case SDL_KEYDOWN:
				s_lastKeyDown = event.key.keysym.sym;
				s_text = event.text.text;
				break;
			case SDL_KEYUP:
				s_lastKeyUp = event.key.keysym.sym;
				if (event.key.keysym.sym == SDLK_ESCAPE)
					Engine::Instance().Running() = false;
				break;
		}
	}
	s_keysCurr = SDL_GetKeyboardState(&s_numKeys);
	s_mouseCurr = SDL_GetMouseState(&s_mousePos.x, &s_mousePos.y);
}

char* EventManager::getText()
{
	return s_text;
}

bool EventManager::KeyHeld(const SDL_Scancode c)
{
	if (s_keysCurr != nullptr)
	{
		if (s_keysCurr[c] == 1)
			return true;
		else
			return false;
	}
	return false;
}

bool EventManager::KeyPressed(const SDL_Scancode c)
{
	return (s_keysCurr[c] > s_keysLast[c]);
}

bool EventManager::KeyReleased(const SDL_Scancode c)
{
	return (s_keysCurr[c] < s_keysLast[c]);
}

int EventManager::LastKeyDown()
{
	return s_lastKeyDown;
}

int EventManager::LastKeyUp()
{
	return s_lastKeyUp;
}

bool EventManager::MouseHeld(const int b)
{
	if (b >= 1 && b <= 3)
		return (s_mouseCurr & SDL_BUTTON(b));
	else
		return false;
}

bool EventManager::MousePressed(const int b)
{
	return ((s_mouseCurr & SDL_BUTTON(b)) > (s_mouseLast & SDL_BUTTON(b)));
}

bool EventManager::MouseReleased(const int b)
{
	return ((s_mouseCurr & SDL_BUTTON(b)) < (s_mouseLast & SDL_BUTTON(b)));
}

void EventManager::SetCursor(const SDL_SystemCursor& c)
{
	SDL_FreeCursor(s_cursor);
	s_cursor = SDL_CreateSystemCursor(c);
	SDL_SetCursor(s_cursor);
}

SDL_Point& EventManager::GetMousePos()
{
	return s_mousePos;
}

SDL_FPoint& EventManager::GetMousePosF()
{
	return s_mousePosF;
}

void EventManager::Quit()
{
	SDL_StopTextInput();
	delete s_keysLast;
}

const Uint8* EventManager::s_keysCurr = nullptr;
Uint8* EventManager::s_keysLast;
int EventManager::s_numKeys;

int EventManager::s_lastKeyDown;
int EventManager::s_lastKeyUp;
char* EventManager::s_text;
SDL_Point EventManager::s_mousePos = { 0,0 };
SDL_FPoint EventManager::s_mousePosF = { 0,0 };
Uint32 EventManager::s_mouseCurr;
Uint32 EventManager::s_mouseLast;
SDL_Cursor* EventManager::s_cursor;