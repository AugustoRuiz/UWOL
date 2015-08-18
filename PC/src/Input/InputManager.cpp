#include "InputManager.h"

InputManager::InputManager()
{
	this->Keys[ActionKeysLeft] = SDLK_LEFT;
	this->Keys[ActionKeysRight] = SDLK_RIGHT;
	this->Keys[ActionKeysUp] = SDLK_LCTRL;
	this->Keys[ActionKeysDown] = SDLK_DOWN;

	this->SetControlMode(Keyboard);
	this->_joystick = NULL;
	this->DefaultKeyJump = SDLK_LCTRL;
	this->DefaultJoyJump = 0; // Button A on Wii

	this->_keyStates = new bool[1024];
	for (int i = 0; i<1024; i++) this->_keyStates[i] = false;

	this->Enabled = true;
	this->_previousHatStatus = SDL_HAT_CENTERED;
}

InputManager *InputManager::GetInstance(void)
{
	return &_instance;
}

bool InputManager::IsKeyPressed(ActionKeys key)
{
	return _keyStates[Keys[key]];
}

void InputManager::SetKeyPressedState(SDL_KeyboardEvent *key)
{
     bool pressed = ( key->type == SDL_KEYDOWN );
	_keyStates[key->keysym.sym] = pressed;
}

void InputManager::RedefineKey(ActionKeys which, SDLKey value) {
	Keys[which] = value;
}

void InputManager::SetControlMode(ControlMode mode) {
	this->_controlMode = mode;
}

Event InputManager::Update(int milliSecs)
{
	Event result;
	SDL_Event event;
	SDL_PollEvent(&event);

	if (SDL_NumJoysticks() > 0 && !SDL_JoystickOpened(0)) {
		this->_joystick = SDL_JoystickOpen(0);
	}

	switch (event.type) {
			// Look for a keypress 
		case SDL_KEYDOWN:
			if (this->Enabled) {
				if (this->_controlMode == Keyboard) {
					this->SetKeyPressedState(&(event.key));
				}
				result.Name = "KEY_DOWN";
				result.Data["key"] = event.key.keysym.sym;
				result.Data["modifiers"] = event.key.keysym.mod;
			}
			break;
		case SDL_KEYUP:
			if (this->Enabled) {
				if (this->_controlMode == Keyboard) {
					this->SetKeyPressedState(&(event.key));
				}
				result.Name = "KEY_UP";
				result.Data["key"] = event.key.keysym.sym;
			}
			break;
		case SDL_JOYBUTTONDOWN:
			if (this->Enabled) {
				if (event.jbutton.which == 0 && event.jbutton.button == JOY_BUTTON_0) {
					if (this->_controlMode == Joystick) {
						SDL_KeyboardEvent ev;
						ev.type = SDL_KEYDOWN;
						ev.keysym.sym = Keys[ActionKeysUp];
						this->SetKeyPressedState(&ev);
					}
					result.Name = "JOY_DOWN";
					result.Data["button"] = event.jbutton.button;
				}
			}
			break;
		case SDL_JOYBUTTONUP:
			if (this->Enabled) {
				if (event.jbutton.which == 0 && event.jbutton.button == JOY_BUTTON_0) {
					if (this->_controlMode == Joystick) {
						SDL_KeyboardEvent ev;
						ev.type = SDL_KEYUP;
						ev.keysym.sym = Keys[ActionKeysUp];
						this->SetKeyPressedState(&ev);
					}
					result.Name = "JOY_UP";
					result.Data["button"] = event.jbutton.button;
				}
			}
			break;
		case SDL_QUIT:
			//Log::WriteLog("Received SDL_QUIT... Ignoring...\n");
			// _running = false;
			break;
		default:
			break;
	}

	if (this->_joystick != NULL) {
		Uint8 hatStatus = SDL_JoystickGetHat(this->_joystick, 0);
		if (hatStatus != _previousHatStatus) {
			result.Name = "JOY_HAT";
			result.Data["hat"] = hatStatus;
		}
		if (this->_controlMode == Joystick) {
			this->setKeyFromHat(hatStatus, SDL_HAT_LEFT, ActionKeysLeft);
			this->setKeyFromHat(hatStatus, SDL_HAT_RIGHT, ActionKeysRight);
			this->setKeyFromHat(hatStatus, SDL_HAT_DOWN, ActionKeysDown);
		}
		_previousHatStatus = hatStatus;
	}

	_lastEvent = result;
	return result;
}

void InputManager::setKeyFromHat(int hatStatus, int hatDirection, ActionKeys key) {
	if ((hatStatus & hatDirection) == hatDirection) {
		SDL_KeyboardEvent ev;
		ev.type = SDL_KEYDOWN;
		ev.keysym.sym = Keys[key];
		this->SetKeyPressedState(&ev);
	}
	else {
		if (this->IsKeyPressed(key)) {
			SDL_KeyboardEvent ev;
			ev.type = SDL_KEYUP;
			ev.keysym.sym = Keys[key];
			this->SetKeyPressedState(&ev);
		}
	}
}

Event InputManager::GetLastEvent() {
	return _lastEvent;
}