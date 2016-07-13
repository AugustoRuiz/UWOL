#pragma once

#include <fstream>
#include "GameData.h"
#include "Stage.h"

#define WAIT_TIME 4000

class Portada : public IGameState
{
public:
	Portada(void);
	~Portada(void);

	// Implementación de IGameState.
	virtual void OnEnter(void) override;
	virtual void OnExit(void) override;
	virtual string Update(Uint32 milliSec, Event & inputEvent) override;
	virtual void Draw(void) override;
	virtual void Dispose(void) override;
	virtual Program * GetProgram(void) override;

private:

	bool _disposed;

	Graphics *_g;

	vector<Frame*> _imagenes;
	Sound* _sonido;

	unsigned int _currentFrame;

	float _currentAlpha;
	int _incrFactor;

	Uint32 _currentTick;

	void UpdateCurrentTexture();

};

