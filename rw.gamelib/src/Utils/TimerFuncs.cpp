#include "Utils/TimerFuncs.h"

int TimerFuncs::GetTicks()
{
	return SDL_GetTicks();
}

void TimerFuncs::Sleep(unsigned int milliSecs) {
	SDL_Delay(milliSecs);
}
