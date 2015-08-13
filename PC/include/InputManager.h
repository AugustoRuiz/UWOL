#pragma once

#include "SDL.h"
#include "Log.h"
#include "UWOL.h"
#include <map>

using namespace std;

typedef map<ActionKeys, SDLKey> KEYMAP;

class InputManager
{
private:
	bool *_keyStates;

public:
	static InputManager * GetInstance(void);
	bool IsKeyPressed(ActionKeys key);
	void SetKeyPressedState(SDL_KeyboardEvent *key);
	void RedefineKey(ActionKeys which, SDLKey value);

protected:
	InputManager(void);
	~InputManager(void) { };

	static InputManager _instance;

private:
	KEYMAP gameKeys;

};
