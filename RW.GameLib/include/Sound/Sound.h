#pragma once

#include "MusicManager.h"

using namespace std;

class Sound
{
private:
	Mix_Chunk* _chunk;

public:
	Sound();
	Sound(string fileName);
	~Sound(void);

	string Name;

	void PlayAsMusic(bool looped);
	void FadeInMusic(bool looped, int mSec);
	void PlayAsFx(bool looped);
};
