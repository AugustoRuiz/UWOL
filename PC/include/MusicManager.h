#pragma once

#include "GameData.h"
#include <map>
#include <string>

using namespace std;

typedef map<string, Mix_Chunk*> MUSICMAP;

class MusicManager
{
private:
	Mix_Chunk* loadMusic(const string &fileName);
	// returns the channel the FX is playing on.
	int playFX(Mix_Chunk* fx, bool looped);
	void playMusic(Mix_Chunk* song, bool looped);
	void fadeInMusic(Mix_Chunk* song, bool looped, int mSec);

public:
	MusicManager(void);
	~MusicManager(void);

	void DeleteMusic();
	void Initialize();
	void Dispose();

	static MusicManager* GetInstance();

	static void LoadMusic(const string &name);

	static void PlayMusic(const string &name, bool looped);
	static void FadeInMusic(const string &name, bool looped, int mSec);
	static void FadeOutMusic(int mSec);
	static int PlayFx(const string &name, bool looped);
	static bool IsPlayingMusic();
	static void StopFX(int channel);
	static void SetMusicVol(int vol);
	static void StopMusic();

protected:
	static MusicManager _instance;

private:
	MUSICMAP cache;
};
