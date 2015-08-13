#pragma once

#include "GameData.h"
#include "Graphics.h"

class CollisionMap
{
	Graphics *_g;

public:
	CollisionMap(void);
	~CollisionMap(void);

	CollisionType Collides(RECTANGLEF *srcRect, VECTOR2F *vel);
	void Initialize(int nCols, int nRows, int cellW, int cellH);
	void LoadMap(SDL_RWops *rw);
	void WriteMap(SDL_RWops *rw);

	int cols;
	int rows;
	int cellWidth;
	int cellHeight;

	CollisionType *map;

	void DebugPaint();

};
