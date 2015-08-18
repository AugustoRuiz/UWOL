#pragma once

#include <fstream>
#include "Graphics.h"
#include "GameData.h"
#include "Stage.h"
#include "IGameState.h"

class Presentacion : public IGameState
{
	float _rTextTop, _gTextTop, _bTextTop, _rTextBot, _gTextBot, _bTextBot, _textAlpha;
	Uint32 _totalTicks;

public:
	Presentacion(void);
	~Presentacion(void);

	// Implementación de IGameState.
	void OnEnter(void) override;
	void OnExit(void) override;
	string Update(Uint32 milliSec, Event & inputEvent) override;
	void Draw(void) override;
	void Dispose(void) override;

private:

	bool _disposed;

	Graphics *_g;

	Frame _frameSombra;

	vector<Frame>::iterator _currentFrame;
	vector<Frame> _logos;

	Coin** _coins;

	float _currentAlpha;
	int _incrFactor;

	Uint32 _currentTick;

	void UpdateCurrentTexture();
};

