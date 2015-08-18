#include "GameOver.h"

GameOver::GameOver()
{
	_g = Graphics::GetInstance();
	this->Name = "GameOver";
	this->_currentFrame = Frame("data/game-over.png");
}

GameOver::~GameOver()
{
}

void GameOver::OnEnter()
{
	MusicManager::PlayMusic("GameOver.ogg", false);
}

void GameOver::OnExit()
{
	MusicManager::FadeOutMusic(300);
}

string GameOver::Update(Uint32 milliSec, Event & inputEvent)
{
	if (inputEvent.Name == "KEY_DOWN" || inputEvent.Name == "JOY_DOWN") {
		return "Presentacion";
	}
	return this->Name;
}

void GameOver::Draw(void)
{
	_g->BlitFrame(this->_currentFrame, 0, 0, this->_currentFrame.Texture->width, this->_currentFrame.Texture->height, false, false);
}

void GameOver::Dispose(void)
{
}
