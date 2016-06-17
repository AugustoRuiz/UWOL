#pragma once

#include "GameData.h"

struct IChaseable
{
public:
	virtual bool ShouldChase() = 0;
	virtual VECTOR2F GetPosition() = 0;
};