#pragma once

#include <fstream>
#include "TextureMgr.h"
#include "GLFuncs.h"
#include "GameData.h"
#include "Stage.h"
#include "IGameState.h"
#include "TextRenderer.h"

class Presentacion : public IGameState
{
public:
	Presentacion(void);
	~Presentacion(void);

	// Implementación de IGameState.
	void Initialize(void);
	string Update(Uint32 milliSec, IGameState *lastState);
	void Draw(void);
	void Dispose(void);

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

