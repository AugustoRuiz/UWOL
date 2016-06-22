#pragma once

#include <SDL2/SDL.h>
#include "Log.h"
#include "UWOL.h"
#include "Event.h"
#include <map>
#include <algorithm>

#ifdef GEKKO
#define JOY_BUTTON_0 2 // Buttons 1 & 2 in WiiMote are 2 and 3 for SDL.
#define JOY_BUTTON_1 3
#else
#define JOY_BUTTON_0 0
#define JOY_BUTTON_1 1
#endif

#define HAT_CENTER SDL_HAT_CENTERED
#define HAT_UP SDL_HAT_UP
#define HAT_DOWN SDL_HAT_DOWN
#define HAT_LEFT SDL_HAT_LEFT
#define HAT_RIGHT SDL_HAT_RIGHT

using namespace std;

//typedef pair<const ActionKeys, const int> ACTIONMAP_ITEM;
//typedef multimap<ActionKeys, int> ACTIONMAP;
//
typedef pair<const int, const ActionKeys> KEYMAP_ITEM;
typedef map<int, ActionKeys> KEYMAP;

class InputManager
{
private:
	bool *_keyStates;
	Event _lastEvent;
	ControlMode _controlMode;
	SDL_Joystick *_joystick;

	int _previousHatStatus;

	void setKeyFromJoyEvent(int hatStatus, int hatDirection, ActionKeys key);

public:
	bool Enabled;
	
	static InputManager * GetInstance(void);
	bool IsKeyPressed(ActionKeys key);
	void SetKeyPressedState (Event &ev);
	void RedefineKey(ActionKeys which, int value, bool replace);
	void SetControlMode(ControlMode mode);

	Event Update(int milliSecs);
	Event GetLastEvent();

	//ACTIONMAP ActionMap;
	KEYMAP KeyMap;

protected:
	InputManager(void);
	~InputManager(void) { };

	static InputManager _instance;

private:

};
