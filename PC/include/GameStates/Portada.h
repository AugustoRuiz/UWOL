#pragma once

#include <fstream>
#include "Graphics.h"
#include "GameData.h"
#include "Stage.h"
#include "IGameState.h"
#include "InputManager.h"
#include "Sound.h"

#define WAIT_TIME 4000

class Portada : public IGameState
{
public:
	Portada(void);
	~Portada(void);

	// Implementación de IGameState.
	virtual void OnEnter(void) override;
	virtual void OnExit(void) override;
	string Update(Uint32 milliSec, Event & inputEvent) override;
	void Draw(void) override;
	void Dispose(void) override;

private:

	bool _disposed;

	Graphics *_g;

	vector<Frame> _imagenes;
	Sound _sonido;

	unsigned int _currentFrame;


	float _currentAlpha;
	int _incrFactor;

	Uint32 _currentTick;

	void UpdateCurrentTexture();
};

