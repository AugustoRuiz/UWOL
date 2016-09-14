#pragma once

#include "../Common.h"
#include "../Input/Event.h"

struct IUpdatable
{
public:
	virtual void Update(Uint32 milliSec, const Event& event) = 0;
	virtual bool UpdateWhenNoCoins(void) = 0;
	virtual ~IUpdatable() {};
};
