#pragma once

#include <fstream>
#include <sstream>
#include <iomanip>
#include "GameData.h"
#include "Room.h"
#include "StatsDraw.h"

using namespace std;

class Stage : public IGameState
{
public:
	Stage(void);
	~Stage(void);

	// Implementación de IGameState.
	virtual void OnEnter(void) override;
	virtual void OnExit(void) override;
	virtual string Update(Uint32 milliSec, Event & inputEvent) override;
	virtual void Draw(void) override;
	virtual void Dispose(void) override;
	virtual Program * GetProgram(void) override;

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

	Frame* _frameSombra;

	float _fadeLevel;
	float _fadeInc;
	bool _fading;

	void DrawTime();
};
