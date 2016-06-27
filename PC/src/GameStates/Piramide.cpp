#include "Piramide.h"

Piramide::Piramide(Stage& stage) : _stage(stage) {
	this->Name = "Piramide";
	this->_g = Graphics::GetInstance();
	this->_frameLadrillo = Frame("data/set-piramide.png");
	this->_frameLogo = Frame("data/texturaUWOL.png");
	this->_frameFlecha = Frame("data/Flecha.png");
	this->_tune = Sound("music/Piramide.ogg");
}

void Piramide::OnEnter() {
	this->_currentAlpha = 0.0f;
	this->_incrFactor = 2;
	this->_stage.Player->initializePlayerData();
	this->_ticks = 0;
	this->_sinValue = 0.0f;
	this->_tune.PlayAsMusic(true);
}

void Piramide::OnExit() {
	MusicManager::FadeOutMusic(300);
}

string Piramide::Update(Uint32 milliSec, Event & inputEvent)
{
	this->_stage.StatsDrawer->Update(milliSec);

	this->_ticks += 0.01f * milliSec;
	while (this->_ticks > TAU) {
		this->_ticks = (float) (this->_ticks - TAU);
	}
	this->_sinValue = sinf(this->_ticks);

	if (this->_currentAlpha < 1.0f) {
		this->_currentAlpha += ((float)this->_incrFactor) *  milliSec * 0.001f;
	}
	if (inputEvent.Name == "KEY_UP" || inputEvent.Name == "JOY_UP") {
		return "Stage";
	}
	return "Piramide";
}

void Piramide::Draw(void)
{
	int logoW = this->_frameLogo.Texture->width * 2, logoH = this->_frameLogo.Texture->height * 2;
	int arrowW = this->_frameFlecha.Texture->width * 2, arrowH = this->_frameFlecha.Texture->height * 2;

	_g->BlitFrameAlpha(this->_frameLogo, (_g->WorldWidth - logoW) / 2, -32, logoW, logoH, 0.33f, false, false);

	this->_stage.StatsDrawer->DrawLives(0, -32, this->_stage.Player->_vidas);
	this->_stage.StatsDrawer->DrawCoins(288, -32, this->_stage.Player->_coinsTaken);
	this->_stage.StatsDrawer->DrawLevel(0, 320, this->_stage.CurrentRoom->GetDepth());
	this->_stage.StatsDrawer->DrawScore(208, 320, this->_stage.Player->GetScore());

	int ladrilloW = 32, ladrilloH = 16;

	int posIniX = _g->WorldWidth / 2;
	int posY = _g->WorldHeight - 192;
	int posX = posIniX;
	int profundidad = 0;

	float r = 1.0f, g = 1.0f, b = 1.0f;
	float rFlecha = (float)(sinf(this->_ticks + 0) * .5 + .5);
	float gFlecha = (float)(sinf(this->_ticks + 2) * .5 + .5);
	float bFlecha = (float)(sinf(this->_ticks + 4) * .5 + .5);

	int arrowX = 0, arrowY = 0;

	for (int i = 0, li = (int)this->_stage.Rooms.size(); i < li; ++i) {
		if (i == profundidad * (profundidad + 1) / 2) {
			++profundidad;
			posIniX -= ladrilloW / 2;
			posX = posIniX;
			posY += ladrilloH;
		}
		if (i == this->_stage.RoomIndex) {
			r = gFlecha; g = bFlecha; b = rFlecha;
			arrowX = (int) (posX + ladrilloW / 2 - (arrowW / 2));
			arrowY = (int) (posY - arrowH + (8 * this->_sinValue));
		}
		else if (this->_stage.Rooms[i]->Completada) {
			r = 1.0f; g = 0.0f; b = 0.0f;
		}
		else {
			r = 0.25f; g = 0.25f; b = 1.0f;
		}
		_g->BlitColoredFrame(this->_frameLadrillo, posX, posY, ladrilloW, ladrilloH, r, g, b, 1.0f, false, false, false);
		posX += ladrilloW;
	}
	_g->BlitColoredFrame(this->_frameFlecha, arrowX, arrowY, arrowW, arrowH, rFlecha, gFlecha, bFlecha, 1.0, false, false, false);
}

void Piramide::Dispose(void)
{
}
