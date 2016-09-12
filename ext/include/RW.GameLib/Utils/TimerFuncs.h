#pragma once

#include "../Common.h"

class TimerFuncs
{
public:
	static int GetTicks();
	static void Sleep(unsigned int milliSecs);
};
