#pragma once

#include <SDL_mixer.h>
#include "GameData.h"
#include <map>
#include <string>

using namespace std;

typedef map<string, Mix_Chunk*> MUSICMAP;

class MusicManager
{
public:
	MusicManager(void);
	~MusicManager(void);

	Mix_Chunk* LoadMusic(const string &fileName);
	void DeleteMusic();
    void PlayMusic(Mix_Chunk* song, bool looped);
	void FadeOutMusic(int mSec);
	void FadeInMusic(Mix_Chunk* song, bool looped, int mSec);
	bool IsPlayingMusic();
	// returns the channel the FX is playing on.
	int PlayFX(Mix_Chunk* fx, bool looped); 
	void Initialize();
	void Dispose();
	void StopMusic();
	void StopFX(int channel);
	void SetMusicVol(int vol);

	static MusicManager* GetInstance();

protected:
	static MusicManager _instance;

private:
	MUSICMAP cache;
};
