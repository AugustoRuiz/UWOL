#include "EndGame.h"

EndGame::EndGame(bool ok): _ok(ok) {
	_g = Graphics::GetInstance();
	if (ok) {
		this->_tune = Sound("music/EndOK.ogg");
		this->Name = "FinJuego_OK";
		this->_currentFrame = Frame("data/fin-good.png");
	}
	else {
		this->_tune = Sound("music/EndKO.ogg");
		this->Name = "FinJuego_KO";
		this->_currentFrame = Frame("data/fin-bad.png");
	}
}

EndGame::~EndGame()
{
}

void EndGame::OnEnter()
{
	this->_tune.PlayAsMusic(true);
}

void EndGame::OnExit()
{
	MusicManager::FadeOutMusic(300);
}

string EndGame::Update(Uint32 milliSec, Event & inputEvent)
{
	if (inputEvent.Name == "KEY_DOWN" || inputEvent.Name == "JOY_DOWN") {
		if (this->_ok) {
			return "GameOver";
		}
		else {
			return "Stage";
		}
	}
	return this->Name;
}

void EndGame::Draw(void)
{
	int width = this->_currentFrame.Texture->width * 2;
	int height = this->_currentFrame.Texture->height * 2;

	_g->BlitCenteredFrameAlpha(this->_currentFrame, width, height, 1.0f, false, false);
}

void EndGame::Dispose(void)
{
}

Program * EndGame::GetProgram(void)
{
	return nullptr;
}
