#pragma once

#include "GameData.h"
#include "Event.h"

struct IGameState {
	string Name;

	virtual void OnEnter() = 0;
	virtual void OnExit() = 0;

	virtual string Update(Uint32 milliSec, Event & inputEvent) = 0;
	virtual void Draw(void) = 0;
	virtual void Dispose(void) = 0;
	
	virtual ~IGameState() { };
};
