#pragma once

#include "IGameState.h"
#include "GameData.h"
#include "Graphics.h"
#include "InputManager.h"
#include "MusicManager.h"

using namespace std;

class Piramide : public IGameState
{
	float _currentAlpha;
	int _incrFactor;
public:
	Piramide();

	virtual void Initialize(void) override;
	virtual string Update(Uint32 milliSec, IGameState * lastState) override;
	virtual void Draw(void) override;
	virtual void Dispose(void) override;

	int NumFase;
};