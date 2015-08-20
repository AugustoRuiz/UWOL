#pragma once

#include <fstream>
#include <sstream>
#include <iomanip>
#include "GameData.h"
#include "Room.h"
#include "IGameState.h"
#include "Graphics.h"
#include "AnimationPlayer.h"
#include "StatsDraw.h"

using namespace std;

class Stage : public IGameState
{
public:
	Stage(void);
	~Stage(void);

	// Implementación de IGameState.
	void OnEnter(void) override;
	void OnExit(void) override;
	string Update(Uint32 milliSec, Event & inputEvent) override;
	void Draw(void) override;
	void Dispose(void) override;

	void GoToRoom(int roomIndex);
	void Restart();

	std::vector<Room*> Rooms;
	Room *CurrentRoom;
	int RoomIndex;

	StatsDraw *StatsDrawer;
	TPlayer *Player;

private:

	bool _disposed;

    Room* loadRooms();
	void disposeRooms();

	Graphics *_g;

	Frame _frameSombra;

	float _fadeLevel;
	float _fadeInc;
	bool _fading;

	void DrawTime();
};
