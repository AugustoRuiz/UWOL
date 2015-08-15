#pragma once

#include <fstream>
#include <sstream>
#include <iomanip>
#include "GameData.h"
#include "Room.h"
#include "IGameState.h"
#include "Graphics.h"
#include "AnimationPlayer.h"

using namespace std;

class Stage : public IGameState
{
public:
	Stage(void);
	~Stage(void);

	// Implementación de IGameState.
	void Initialize(void);
	string Update(Uint32 milliSec, IGameState *lastState);
	void Draw(void);
	void Dispose(void);

	void GoToRoom(int roomIndex);
private:

	bool _disposed;

    Room* loadRooms();
	void disposeRooms();

	Graphics *_g;

	AnimationPlayer _coinAnimPlayer;
	AnimationPlayer _uwolAnimPlayer;

	Frame _frameSombra;

	bool _rotatingCoin;
	bool _movingUwol;

	int _ultimoEstadoUwol;

	int _profundidad;
	int _currentRoomIdx;

	float _fadeLevel;
	float _fadeInc;
	bool _fading;

	std::vector<Room*> rooms;
	Room *_currentRoom;

	Player *_player;

	void DrawLives();
	void DrawTime();
	void DrawCoins();
	void DrawLevel();
	void DrawScore();
};
