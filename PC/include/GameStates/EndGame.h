#pragma once

#include "IGameState.h"
#include "GameData.h"
#include "Graphics.h"
#include "InputManager.h"
#include "MusicManager.h"

class EndGame : public IGameState
{
	bool _ok;
	Frame _currentFrame;
	string _tuneName;
	Graphics *_g;
public:
	EndGame(bool ok);
	~EndGame();

	// Inherited via IGameState
	virtual void OnEnter() override;
	virtual void OnExit() override;
	virtual string Update(Uint32 milliSec, Event & inputEvent) override;
	virtual void Draw(void) override;
	virtual void Dispose(void) override;
};

