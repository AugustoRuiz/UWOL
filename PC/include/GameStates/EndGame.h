#pragma once

#include "GameData.h"

class EndGame : public IGameState
{
public:
	EndGame(bool ok);
	~EndGame();

	// Inherited via IGameState
	virtual void OnEnter() override;
	virtual void OnExit() override;
	virtual string Update(Uint32 milliSec, Event & inputEvent) override;
	virtual void Draw(void) override;
	virtual void Dispose(void) override;
	virtual Program * GetProgram(void) override;
private:
	bool _ok;
	bool _disposed;
	Frame* _currentFrame;
	Sound* _tune;
	Graphics *_g;
};

