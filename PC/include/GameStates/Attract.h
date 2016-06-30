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

class Attract : public IGameState
{
public:
	Attract();

	void SetAttractData(const vector<Uint32> &eventBuffer);
	void SetRooms(vector<Room*> &rooms);
	void SetPlayer(TPlayer* player);

	// Inherited via IGameState
	virtual void OnEnter() override;
	virtual void OnExit() override;
	virtual string Update(Uint32 milliSec, Event & inputEvent) override;
	virtual void Draw(void) override;
	virtual void Dispose(void) override;
	virtual Program* GetProgram() override;

protected:
	TPlayer* _player;
	Program _program;
	
	Frame _frameNoise, _frameSombra;

	vector<vector<Uint32>*> _eventsByRoom;
	vector<Uint32> _currentRoomEvts;
	vector<Uint32>::iterator _evtBufferIterator;
	
	vector<Room*> _rooms;
	Room* _currentRoom;
	
	Uint32 _totalTicks;
	char _incrFactor;
	float _currentAlpha;
};