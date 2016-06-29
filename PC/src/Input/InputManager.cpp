#include "InputManager.h"

InputManager::InputManager()
{
	//this->ActionMap.insert(ACTIONMAP_ITEM(ActionKeysLeft, SDLK_LEFT));
	//this->ActionMap.insert(ACTIONMAP_ITEM(ActionKeysLeft, SDLK_o));

	//this->ActionMap.insert(ACTIONMAP_ITEM(ActionKeysRight, SDLK_RIGHT));
	//this->ActionMap.insert(ACTIONMAP_ITEM(ActionKeysRight, SDLK_p));

	//this->ActionMap.insert(ACTIONMAP_ITEM(ActionKeysUp, SDLK_LCTRL));
	//this->ActionMap.insert(ACTIONMAP_ITEM(ActionKeysUp, SDLK_RCTRL));
	//this->ActionMap.insert(ACTIONMAP_ITEM(ActionKeysUp, SDLK_q));

	//this->ActionMap.insert(ACTIONMAP_ITEM(ActionKeysDown, SDLK_DOWN));
	//this->ActionMap.insert(ACTIONMAP_ITEM(ActionKeysDown, SDLK_a));

	//this->ActionMap.insert(ACTIONMAP_ITEM(ActionKeysExit, SDLK_ESCAPE));
	//this->ActionMap.insert(ACTIONMAP_ITEM(ActionKeysStopRecording, SDLK_r));
	//this->ActionMap.insert(ACTIONMAP_ITEM(ActionKeysPause, SDLK_h));
	//this->ActionMap.insert(ACTIONMAP_ITEM(ActionKeysScanlines, SDLK_F1));
	//this->ActionMap.insert(ACTIONMAP_ITEM(ActionKeysAltScanlines, SDLK_F2));
	//this->ActionMap.insert(ACTIONMAP_ITEM(ActionKeysDebug, SDLK_d));

	this->KeyMap[SDLK_LEFT] = ActionKeysLeft;
	this->KeyMap[SDLK_o] = ActionKeysLeft;

	this->KeyMap[SDLK_RIGHT] = ActionKeysRight;
	this->KeyMap[SDLK_p] = ActionKeysRight;

	this->KeyMap[SDLK_LCTRL] = ActionKeysUp;
	this->KeyMap[SDLK_RCTRL] = ActionKeysUp;
	this->KeyMap[SDLK_q] = ActionKeysUp;

	this->KeyMap[SDLK_DOWN] = ActionKeysDown;
	this->KeyMap[SDLK_a] = ActionKeysDown;

	this->KeyMap[SDLK_ESCAPE] = ActionKeysExit;
	this->KeyMap[SDLK_r] = ActionKeysStopRecording;
	this->KeyMap[SDLK_h] = ActionKeysPause;
	this->KeyMap[SDLK_F1] = ActionKeysScanlines;
	this->KeyMap[SDLK_F2] = ActionKeysAltScanlines;
	this->KeyMap[SDLK_F3] = ActionKeysAliasing;
	this->KeyMap[SDLK_d] = ActionKeysDebug;
	this->KeyMap[SDLK_PLUS] = ActionKeysNextScreen;
	this->KeyMap[SDLK_MINUS] = ActionKeysPreviousScreen;
	this->KeyMap[SDLK_e] = ActionKeysAddCoins;
	this->KeyMap[SDLK_i] = ActionKeysToggleInertia;

	this->SetControlMode(Keyboard);
	this->_joystick = NULL;

	this->_keyStates = new bool[ACTION_KEYS_COUNT];
	for (int i = 0; i < ACTION_KEYS_COUNT; i++) this->_keyStates[i] = false;

	this->Enabled = true;
	this->_previousHatStatus = HAT_CENTER;
}

InputManager *InputManager::GetInstance(void)
{
	return &_instance;
}

bool InputManager::IsKeyPressed(ActionKeys key)
{
	return _keyStates[key];
}

void InputManager::AddFakeEvent(const Event &ev) {
	this->_fakeEvents.push_back(ev);
}

void InputManager::SetKeyPressedState(Event &ev) {
	bool pressed = (ev.Name == "KEY_DOWN");
	_keyStates[ev.Data["key"].asInt()] = pressed;
}

void InputManager::RedefineKey(ActionKeys which, int value, bool replace) {
	if (replace) {
		// Buscamos todas las entradas del KeyMap que tienen la actionkey which.
		KEYMAP::iterator it = KeyMap.begin();
		do {
			if (it->second == which) {
				KeyMap.erase(it->first);
			}
			if (it != KeyMap.end()) { ++it; }
		} while (it != KeyMap.end());
	}
	KeyMap.insert(KEYMAP_ITEM(value, which));
}

void InputManager::SetControlMode(ControlMode mode) {
	this->_controlMode = mode;
}

Event InputManager::Update(int milliSecs)
{
	Event result;
	SDL_Event event;

	if (this->_fakeEvents.size() > 0) {
		result = this->_fakeEvents[0];
		this->_fakeEvents.erase(this->_fakeEvents.begin());
		if (this->_controlMode == Keyboard) {
			this->SetKeyPressedState(result);
		}
	}
	else {
		SDL_PollEvent(&event);

		if (SDL_NumJoysticks() > 0 && (this->_joystick == NULL)) {
			this->_joystick = SDL_JoystickOpen(0);
		}

		switch (event.type) {
			// Look for a keypress 
		case SDL_KEYDOWN:
			if (this->Enabled) {
				int key = event.key.keysym.sym;
				if (this->KeyMap.find(key) != this->KeyMap.end()) {
					result.Name = "KEY_DOWN";
					result.Data["key"] = this->KeyMap[key];
					if (this->_controlMode == Keyboard) {
						this->SetKeyPressedState(result);
					}
				}
			}
			break;
		case SDL_KEYUP:
			if (this->Enabled) {
				int key = event.key.keysym.sym;
				if (this->KeyMap.find(key) != this->KeyMap.end()) {
					result.Name = "KEY_UP";
					result.Data["key"] = this->KeyMap[key];
					if (this->_controlMode == Keyboard) {
						this->SetKeyPressedState(result);
					}
				}
			}
			break;
		case SDL_JOYBUTTONDOWN:
			if (this->Enabled) {
				if (event.jbutton.which == 0 && event.jbutton.button >= 0 && event.jbutton.button <= 4) {
					if (this->_controlMode == Joystick) {
						this->setKeyFromJoyEvent(1, 1, ActionKeysUp);
					}
					result.Name = "JOY_DOWN";
					result.Data["button"] = event.jbutton.button;
				}
			}
			break;
		case SDL_JOYBUTTONUP:
			if (this->Enabled) {
				if (event.jbutton.which == 0 && event.jbutton.button >= 0 && event.jbutton.button <= 4) {
					if (this->_controlMode == Joystick) {
						this->setKeyFromJoyEvent(0, 0, ActionKeysUp);
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
				this->setKeyFromJoyEvent(hatStatus, HAT_LEFT, ActionKeysLeft);
				this->setKeyFromJoyEvent(hatStatus, HAT_RIGHT, ActionKeysRight);
				this->setKeyFromJoyEvent(hatStatus, HAT_DOWN, ActionKeysDown);
			}
			_previousHatStatus = hatStatus;
		}
	}

	_lastEvent = result;
	return result;
}

void InputManager::setKeyFromJoyEvent(int status, int mask, ActionKeys key) {
	if ((mask) && ((status & mask) == mask)) {
		Event ev;
		ev.Name = "KEY_DOWN";
		ev.Data["key"] = key;
		this->SetKeyPressedState(ev);
	}
	else {
		if (this->IsKeyPressed(key)) {
			Event ev;
			ev.Name = "KEY_UP";
			ev.Data["key"] = key;
			this->SetKeyPressedState(ev);
		}
	}
}

Event InputManager::GetLastEvent() {
	return _lastEvent;
}