#include "EndGame.h"

EndGame::EndGame(bool ok): _ok(ok) {
	_g = Graphics::GetInstance();
	if (ok) {
		this->_tune = new Sound("music/EndOK.ogg");
		this->Name = "FinJuego_OK";
		this->_currentFrame = new Frame("data/fin-good.png");
	}
	else {
		this->_tune = new Sound("music/EndKO.ogg");
		this->Name = "FinJuego_KO";
		this->_currentFrame = new Frame("data/fin-bad.png");
	}
	this->_disposed = false;
}

EndGame::~EndGame() {
	this->Dispose();
}

void EndGame::OnEnter() {
	this->_tune->PlayAsMusic(true);
}

void EndGame::OnExit() {
	MusicManager::FadeOutMusic(300);
}

string EndGame::Update(Uint32 milliSec, Event & inputEvent) {
	if (inputEvent.Name == "KEY_UP" || inputEvent.Name == "JOY_UP") {
		if (this->_ok) {
			return "GameOver";
		}
		else {
			return "Stage";
		}
	}
	return this->Name;
}

void EndGame::Draw(void) {
	int width = this->_currentFrame->Texture->width * 2;
	int height = this->_currentFrame->Texture->height * 2;

	_g->BlitCenteredFrameAlpha(this->_currentFrame, width, height, 1.0f, false, false);
}

void EndGame::Dispose(void) {
	if (!this->_disposed) {
		this->_disposed = true;
		delete this->_tune;
		delete this->_currentFrame;
	}
}

Program * EndGame::GetProgram(void) {
	return nullptr;
}
