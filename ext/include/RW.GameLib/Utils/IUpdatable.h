#pragma once

#include "Common.h"

struct IUpdatable
{
public:
	virtual void Update(Uint32 milliSec) = 0;
	virtual bool UpdateWhenNoCoins(void) = 0;
	virtual ~IUpdatable() {};
};
