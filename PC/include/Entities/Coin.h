#pragma once

#include "GameData.h"

class Coin : public IDrawable, public IUpdatable
{
public:
	Coin(void);
	~Coin(void);

	void setTileSize(VECTOR2 tileSize);
	void setPos(char tileX, char tileY);

	void Draw() override;
	void DrawInPos(int pX, int pY, float alpha);
	void DrawShadow(void) override;

	void Update(Uint32 milliSec) override;

	void Initialize();
	void Dispose();

	bool DrawWhenNoCoins() override;
    bool UpdateWhenNoCoins() override;

	int _x;
	float _y;

	VECTOR2F _speed;
	float _rotationFactor;

	void SetFrame(int frame);
	void SetTicks(int ticks);

private:
	AnimationPlayer _animPlayer;

	VECTOR2 _tileSize;

	Graphics *_g;

	bool _disposed;
};
