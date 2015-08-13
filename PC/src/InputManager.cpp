#include "InputManager.h"

InputManager::InputManager()
{
	gameKeys[ActionKeysLeft] = SDLK_LEFT;
	gameKeys[ActionKeysRight] = SDLK_RIGHT;
	gameKeys[ActionKeysUp] = SDLK_LCTRL;
	gameKeys[ActionKeysDown] = SDLK_DOWN;
}

InputManager *InputManager::GetInstance(void)
{
	if (! _instance._keyStates)
	{
		_instance._keyStates = new bool[1024];

		for(int i=0; i<1024; i++) _instance._keyStates[i] = false;
	}

	return &_instance;
}

bool InputManager::IsKeyPressed(ActionKeys key)
{
	return _keyStates[gameKeys[key]];
}

void InputManager::SetKeyPressedState(SDL_KeyboardEvent *key)
{
     bool pressed = ( key->type == SDL_KEYDOWN );
          
	_keyStates[key->keysym.sym] = pressed;
}

void InputManager::RedefineKey(ActionKeys which, SDLKey value)
{
	gameKeys[which] = value;
}
