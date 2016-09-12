#pragma once

#include "GameData.h"
#include "Stage.h"
#include <math.h>

using namespace std;

class Piramide : public IGameState
{
	float _sinValue;
	float _ticks;
	float _currentAlpha;
	int _incrFactor;

	Frame* _frameLadrillo;
	Frame* _frameLogo;
	Frame* _frameFlecha;

	Sound* _tune;

	Graphics* _g;
	Stage* _stage;

public:
	Piramide(Stage *stage);

	virtual void OnEnter(void) override;
	virtual void OnExit(void) override;
	virtual string Update(Uint32 milliSec, Event &inputEvent) override;
	virtual void Draw(void) override;
	virtual void Dispose(void) override;
	virtual Program * GetProgram(void) override;

	int NumFase;

};