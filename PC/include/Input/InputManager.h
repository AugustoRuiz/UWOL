#pragma once

#include <SDL/SDL.h>
#include "Log.h"
#include "UWOL.h"
#include "Event.h"
#include <map>

#define JOY_BUTTON_0 0

using namespace std;

typedef map<ActionKeys, SDLKey> KEYMAP;

class InputManager
{
private:
	bool *_keyStates;
	Event _lastEvent;
	ControlMode _controlMode;
	SDL_Joystick *_joystick;

	int _previousHatStatus;

	void setKeyFromHat(int hatStatus, int hatDirection, ActionKeys key);

public:
	bool Enabled;
	
	static InputManager * GetInstance(void);
	bool IsKeyPressed(ActionKeys key);
	void SetKeyPressedState(SDL_KeyboardEvent *key);
	void RedefineKey(ActionKeys which, SDLKey value);
	void SetControlMode(ControlMode mode);

	Event Update(int milliSecs);
	Event GetLastEvent();

	KEYMAP Keys;

	int DefaultKeyJump;
	int DefaultJoyJump;

protected:
	InputManager(void);
	~InputManager(void) { };

	static InputManager _instance;

private:

};
