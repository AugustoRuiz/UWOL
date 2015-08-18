#include "Piramide.h"

Piramide::Piramide(Stage& stage) : _stage(stage) {
	this->Name = "Piramide";
	this->_frameLadrillo = Frame("data/set-piramide.png");
	this->_frameLogo = Frame("data/texturaUWOL.png");
	this->_g = Graphics::GetInstance();
}

void Piramide::OnEnter() {
	this->_currentAlpha = 1.0f;
	this->_incrFactor = 2;
	this->_stage.Player->initializePlayerData();
	MusicManager::PlayMusic("music/Piramide.wav", true);
}

void Piramide::OnExit() {
	MusicManager::FadeOutMusic(300);
}

string Piramide::Update(Uint32 milliSec, Event & inputEvent)
{
	this->_stage.StatsDrawer->Update(milliSec);

	if (this->_currentAlpha < 1.0f) {
		this->_currentAlpha += ((float)this->_incrFactor) *  milliSec * 0.001f;
	}
	if (inputEvent.Name == "KEY_DOWN" || inputEvent.Name == "JOY_DOWN") {
		return "Stage";
	}
	return "Piramide";
}

void Piramide::Draw(void)
{
	int logoW = 512, logoH = 256;
	_g->BlitFrameAlpha(this->_frameLogo, (_g->WorldWidth - logoW) / 2, -32, logoW, logoH, 0.25, false, false);

	this->_stage.StatsDrawer->DrawLives(0, -32, this->_stage.Player->_vidas);
	this->_stage.StatsDrawer->DrawCoins(288, -32, this->_stage.Player->_coinsTaken);
	this->_stage.StatsDrawer->DrawLevel(0, 320, this->_stage.CurrentRoom->Depth);
	this->_stage.StatsDrawer->DrawScore(208, 320, this->_stage.Player->_score);

	int ladrilloW = 32, ladrilloH = 16;

	int posIniX = _g->WorldWidth / 2, posY = logoH / 2;
	int posX = posIniX;
	int profundidad = 0;

	float r = 1.0f, g = 1.0f, b = 1.0f;

	for (int i = 0, li = this->_stage.Rooms.size(); i < li; ++i) {
		if (i == profundidad * (profundidad + 1) / 2) {
			++profundidad;
			posIniX -= ladrilloW / 2;
			posX = posIniX;
			posY += ladrilloH;
		}
		if (i == this->_stage.RoomIndex) {
			r = 1.0f; g = 0.0f; b = 0.0f;
		}
		else if (this->_stage.Rooms[i]->Completada) {
			r = 0.5f; g = 0.5f; b = 1.0f;
		}
		else {
			r = 1.0f; g = 1.0f; b = 1.0f;
		}
		_g->BlitColoredFrame(this->_frameLadrillo, posX, posY, ladrilloW, ladrilloH, r, g, b, 1.0f, false, false, false);
		posX += ladrilloW;
	}

}

void Piramide::Dispose(void)
{
}
