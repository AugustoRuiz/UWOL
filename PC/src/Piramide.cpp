#include "Piramide.h"

Piramide::Piramide() {
	this->NumFase = 0;
}

void Piramide::Initialize() {
	this->_currentAlpha = 1.0f;
	this->_incrFactor = 2;
	MusicManager::PlayMusic("music/Piramide.ogg", true);
}

string Piramide::Update(Uint32 milliSec, IGameState * lastState)
{
	if (this->_currentAlpha < 1.0f) {
		this->_currentAlpha += ((float)this->_incrFactor) *  milliSec * 0.001f;
	}

	InputManager *input = InputManager::GetInstance();
	Event evt = input->GetLastEvent();
	if (evt.Name == "KEY_DOWN" || evt.Name == "JOY_DOWN") {
		MusicManager::FadeOutMusic(500);
		return "Stage";
	}
	return "Portada";
}

void Piramide::Draw(void)
{
}

void Piramide::Dispose(void)
{
}
