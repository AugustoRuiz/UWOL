#pragma once

#include "GameData.h"

struct IGameState {
	string Name;

	virtual void Initialize(void) = 0;
	virtual string Update(Uint32 milliSec, IGameState *lastState) = 0;
	virtual void Draw(void) = 0;
	virtual void Dispose(void) = 0;
	virtual ~IGameState() { };
};
