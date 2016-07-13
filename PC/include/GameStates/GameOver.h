#pragma once

#include "GameData.h"
#include "Game.h"

class GameOver : public IGameState
{
public:
	GameOver();
	~GameOver();

	// Inherited via IGameState
	virtual void OnEnter() override;
	virtual void OnExit() override;
	virtual string Update(Uint32 milliSec, Event & inputEvent) override;
	virtual void Draw(void) override;
	virtual void Dispose(void) override;
	virtual Program * GetProgram(void) override;

private:
	bool _disposed;

	Frame* _currentFrame;
	Sound* _music;
	Graphics *_g;

	unsigned int _totalMsecs;
};

