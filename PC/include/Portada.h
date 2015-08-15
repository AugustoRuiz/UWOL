#pragma once

#include <fstream>
#include "Graphics.h"
#include "GameData.h"
#include "Stage.h"
#include "IGameState.h"
#include "TextRenderer.h"

#define WAIT_TIME 4000

class Portada : public IGameState
{
public:
	Portada(void);
	~Portada(void);

	// Implementación de IGameState.
	void Initialize(void);
	string Update(Uint32 milliSec, IGameState *lastState);
	void Draw(void);
	void Dispose(void);

private:

	bool _disposed;

	Graphics *_g;

	vector<Frame> _imagenes;
	vector<string> _sonidos;

	int _currentFrame;


	float _currentAlpha;
	int _incrFactor;

	Uint32 _currentTick;

	void UpdateCurrentTexture();
};

