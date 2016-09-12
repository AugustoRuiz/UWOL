#include "Sound/Sound.h"

Sound::Sound() : Name("") {
	this->_chunk = nullptr;
}

Sound::Sound(string fileName) : Name(fileName) {
	this->_chunk = MusicManager::GetInstance()->LoadMusic(fileName);
}

Sound::~Sound(void) {
};

void Sound::PlayAsMusic(bool looped) {
	if (nullptr != this->_chunk) {
		MusicManager::GetInstance()->PlayMusic(this->_chunk, looped);
	}
};

void Sound::FadeInMusic(bool looped, int mSec) {
	if (nullptr != this->_chunk) {
		MusicManager::GetInstance()->FadeInMusic(this->_chunk, looped, mSec);
	}
};

void Sound::PlayAsFx(bool looped) {
	if (nullptr != this->_chunk) {
		MusicManager::GetInstance()->PlayFX(this->_chunk, looped);
	}
};
