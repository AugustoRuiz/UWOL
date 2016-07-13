#pragma once

#include "../Common.h"
#include "../Utils/Log.h"
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
	void Initialize();
	void Dispose();

	static MusicManager* GetInstance();

	// returns the channel the FX is playing on.
	int PlayFX(Mix_Chunk* fx, bool looped);
	void PlayMusic(Mix_Chunk* song, bool looped);
	void FadeInMusic(Mix_Chunk* song, bool looped, int mSec);

	static void FadeOutMusic(int mSec);
	static bool IsPlayingMusic();
	static void StopFX(int channel);
	static void SetMusicVol(int vol);
	static void StopMusic();

protected:
	static MusicManager _instance;

private:
	MUSICMAP cache;
};
