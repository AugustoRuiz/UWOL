#include "MusicManager.h"

MusicManager::MusicManager(void)
{
}

MusicManager::~MusicManager(void)
{
}

Mix_Chunk* MusicManager::loadMusic(const string &sFileName)
{
	Mix_Chunk* result = NULL;

	Log::Out << "MusicManager: Loading sound " << sFileName << "s... " << endl;

	if(cache.find(sFileName) != cache.end())
	{
		Log::Out << "IN CACHE!" << endl;
		result = cache[sFileName];
	}
	else
	{
		result = Mix_LoadWAV(sFileName.c_str());

		if(result == NULL)
		{
			Log::Out << "SDL couldn't load sound " << sFileName << endl;
		}
		else
		{
			cache[sFileName] = result;
			Log::Out << "loaded!" << endl;
		}
	}

	return result;
}

void MusicManager::DeleteMusic()
{
	MUSICMAP::iterator iter;

    Log::Out << "MusicMgr: Deleting sound." << endl;
	iter = cache.begin();
	while(iter != cache.end())
	{
		Mix_FreeChunk(iter->second);
		iter++;
	}
	cache.clear();
}

MusicManager* MusicManager::GetInstance()
{
	return &_instance;
}

void MusicManager::playMusic(Mix_Chunk* song, bool looped)
{
	if(looped)
	{
		Mix_PlayChannel(0, song, -1);
	}
	else
	{
		Mix_PlayChannel(0, song, 0);
	}
}

void MusicManager::FadeOutMusic(int mSec)
{
	Mix_FadeOutChannel(0, mSec);
}

void MusicManager::fadeInMusic(Mix_Chunk* song, bool looped, int mSec)
{
	Mix_FadeInChannel(0, song, looped, mSec);
}

bool MusicManager::IsPlayingMusic()
{
	return (Mix_Playing(0) != 0);
}

int MusicManager::playFX(Mix_Chunk* fx, bool looped)
{
	int channelNo;
	if(looped)
	{
		channelNo = Mix_PlayChannel(-1, fx, -1);
	}
	else
	{
		channelNo = Mix_PlayChannel(-1, fx, 0);
	}
	return channelNo;
}

void MusicManager::StopFX(int channel)
{
	Mix_HaltChannel(channel);
}

void MusicManager::StopMusic()
{
	Mix_HaltChannel(0);
}

void MusicManager::SetMusicVol(int vol)
{
	Mix_Volume(0, vol);
}

void MusicManager::Initialize()
{
	Log::Out << "Initializing MusicManager...";
	if(Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 1024))
	{
		Log::Out << "Error initializing SDL_Mixer." << endl << Mix_GetError() << endl;
		exit(1);
	}
	Log::Out << "Ok" << endl;
}

void MusicManager::Dispose()
{
	Mix_CloseAudio();
}

void MusicManager::PlayMusic(const string &name, bool looped)
{
	MusicManager *inst = MusicManager::GetInstance();
	inst->playMusic(inst->loadMusic(name), looped);
}

int MusicManager::PlayFx(const string &name, bool looped)
{
	MusicManager *inst = MusicManager::GetInstance();
	return inst->playFX(inst->loadMusic(name), looped);
}

void MusicManager::FadeInMusic(const string &name, bool looped, int mSec)
{
	MusicManager *inst = MusicManager::GetInstance();
	inst->fadeInMusic(inst->loadMusic(name), looped, mSec);
}

void MusicManager::LoadMusic(const string &name) {
	MusicManager::GetInstance()->loadMusic(name);
}