#pragma once

#include "IGameState.h"
#include "GameData.h"
#include "Event.h"
#include "Graphics.h"
#include "Frame.h"
#include "MusicManager.h"

class GameOver : public IGameState
{
	Frame _currentFrame;
	Graphics *_g;

public:
	GameOver();
	~GameOver();

	// Inherited via IGameState
	virtual void OnEnter() override;
	virtual void OnExit() override;
	virtual string Update(Uint32 milliSec, Event & inputEvent) override;
	virtual void Draw(void) override;
	virtual void Dispose(void) override;
};

