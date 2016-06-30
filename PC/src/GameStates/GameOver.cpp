#include "GameOver.h"

GameOver::GameOver()
{
	_g = Graphics::GetInstance();
	this->Name = "GameOver";
	this->_currentFrame = Frame("data/game-over.png");
	this->_music = Sound("music/GameOver.ogg");
}

GameOver::~GameOver()
{
}

void GameOver::OnEnter()
{
	this->_music.PlayAsMusic(false);
}

void GameOver::OnExit()
{
	MusicManager::FadeOutMusic(300);
	Game::GetInstance()->Restart();
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
	int width = this->_currentFrame.Texture->width * 2;
	int height = this->_currentFrame.Texture->height * 2;
	
	_g->BlitCenteredFrameAlpha(this->_currentFrame, width, height, 1.0f, false, false);
}

void GameOver::Dispose(void)
{
}

Program * GameOver::GetProgram(void)
{
	return nullptr;
}
