#include "EndGame.h"

EndGame::EndGame(bool ok): _ok(ok) {
	_g = Graphics::GetInstance();
	if (ok) {
		this->_tuneName = "EndOK.ogg";
		this->Name = "FinJuego_OK";
		this->_currentFrame = Frame("data/fin-good.png");
	}
	else {
		this->_tuneName = "EndKO.ogg";
		this->Name = "FinJuego_KO";
		this->_currentFrame = Frame("data/fin-bad.png");
	}
}

EndGame::~EndGame()
{
}

void EndGame::OnEnter()
{
	if (this->_tuneName != "") {
		MusicManager::PlayMusic(this->_tuneName, true);
	}
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
	_g->BlitFrame(this->_currentFrame, 0, 0, this->_currentFrame.Texture->width, this->_currentFrame.Texture->height, false, false);
}

void EndGame::Dispose(void)
{
}
