#pragma once

#include "GameData.h"
#include "IDrawable.h"
#include "IUpdatable.h"
#include "AnimationPlayer.h"
#include "Graphics.h"

class Coin : public IDrawable, public IUpdatable
{
public:
	Coin(void);
	~Coin(void);

	void setTileSize(VECTOR2 tileSize);
	void setPos(char tileX, char tileY);

	void Draw();
	void DrawInPos(int pX, int pY, float alpha);
	void DrawShadow(void);

	void Update(Uint32 milliSec);

	void Initialize();
	void Dispose();

	bool DrawWhenNoCoins();
    bool UpdateWhenNoCoins();

	int _x;
	int _y;

	VECTOR2F _speed;

	void SetFrame(int frame);
	void SetTicks(int ticks);

private:
	AnimationPlayer _animPlayer;

	VECTOR2 _tileSize;

	Graphics *_g;

	bool _disposed;
};
