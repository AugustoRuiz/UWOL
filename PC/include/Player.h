#pragma once

#include "GameData.h"
#include "CollisionMap.h"
#include "IDrawable.h"
#include "IUpdatable.h"
#include "Graphics.h"
#include "AnimationPlayer.h"
#include "InputManager.h"
#include "MusicManager.h"

class Player : public IDrawable, public IUpdatable
{
public:
	Player(void);
	~Player(void);

	void setTileSize(VECTOR2 tileSize);

	bool DrawWhenNoCoins();
	bool UpdateWhenNoCoins();

	void Draw();
	void DrawShadow(void);

	void setPosition(char tileX, char tileY);
	void setAlpha(float alpha);
	void setMap(CollisionMap *map);
	void setEstado(int estado);
	int getEstado();

	void Update(Uint32 milliSec);

	void Initialize();
	void Dispose();

	void initializePlayerData();
	void getTiles(int &tileX, int &tileY, int &tileX2, int &tileY2);

	void DebugPaint();

	float _x, _y;

	int _vidas;
	int _score;
	int _coinsTaken;

	RECTANGLEF _posRect;
private:

	AnimationPlayer _animPlayer;
	InputManager *_input;

	VECTOR2 _tileSize;
	RECTANGLEF _colRect;

	int _estado;

	Graphics *_graphics;

	CollisionMap *_map;

	float _vx, _vy;
	float _g, _ax, _rx;
	float _salto;
	int _contSalto;
	bool _saltando;
	int _frame;
	int _contParpadeo;
	bool _visibleParpadeo;
	Facing _facing;
	float _alpha;
	bool _disposed;
	unsigned char _lastStep;

	bool Collides(int tileX, int tileY);
	void checkInput(Uint32 milliSec);
	void checkMapCollisions(Uint32 milliSec);

	void setAnimation(string name);
};
