#pragma once

#include "GameData.h"

struct IDrawable
{
public:
	virtual void Draw(void) = 0;
	virtual void DrawShadow(void) = 0;
	virtual bool DrawWhenNoCoins(void) = 0;
	virtual ~IDrawable() { };
};
