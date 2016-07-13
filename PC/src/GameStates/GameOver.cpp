#include "GameOver.h"

GameOver::GameOver() {
	_g = Graphics::GetInstance();
	this->Name = "GameOver";
	this->_currentFrame = new Frame("data/game-over.png");
	this->_music = new Sound("music/GameOver.ogg");
	this->_disposed = false;
}

GameOver::~GameOver() {
	this->Dispose();
}

void GameOver::OnEnter() {
	this->_music->PlayAsMusic(false);
	this->_totalMsecs = 0;
}

void GameOver::OnExit() {
	MusicManager::FadeOutMusic(300);
	Game::GetInstance()->Restart();
}

string GameOver::Update(Uint32 milliSec, Event & inputEvent)
{
	this->_totalMsecs += milliSec;
	if (inputEvent.Name == "KEY_UP" || inputEvent.Name == "JOY_UP") {
		return "Presentacion";
	}
	if (this->_totalMsecs > 10000) {
		return "Presentacion";
	}
	return this->Name;
}

void GameOver::Draw(void)
{
	int width = this->_currentFrame->Texture->width * 2;
	int height = this->_currentFrame->Texture->height * 2;
	_g->BlitCenteredFrameAlpha(this->_currentFrame, width, height, 1.0f, false, false);
}

void GameOver::Dispose(void)
{
	if (!this->_disposed) {
		this->_disposed = true;
		delete this->_currentFrame;
		delete this->_music;
	}
}

Program * GameOver::GetProgram(void)
{
	return nullptr;
}
