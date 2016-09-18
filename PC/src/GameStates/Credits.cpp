#include "Credits.h"

#define UWOL_VX 2.5

Credits::Credits(void)
{
	this->Name = "Credits";
	this->_g = Graphics::GetInstance();

	this->_uwolIcon = new Frame("data/texturaUWOL.png");

	this->_tune = new Sound("music/Ghosts3.ogg");
	this->_shadow = new Frame("data/TileSombra.png");

	this->_rTextTop = this->_gTextTop = this->_bTextTop = 0.7f;
	this->_rTextBot = this->_gTextBot = this->_bTextBot = 1.0f;

	this->_rTitleTop = 75.0f / 256.0f - 0.15f;
	this->_gTitleTop = 180.0f / 256.0f - 0.15f;
	this->_bTitleTop = 24.0f / 256.0f - 0.15f;
	this->_rTitleBot = 95.0f / 256.0f;
	this->_gTitleBot = 234.0f / 256.0f;
	this->_bTitleBot = 26.0f / 256.0f;

	this->_rGoldTop = 245.0f / 256.0f - 0.15f;
	this->_gGoldTop = 151.0f / 256.0f - 0.15f;
	this->_bGoldTop = 0.0f;
	this->_rGoldBot = 254.0f / 256.0f;
	this->_gGoldBot = 255.0f / 256.0f;
	this->_bGoldBot = 61.0f / 256.0f;

	this->_textSize = 16;
	this->_maxPosY = 10000;

	this->_fanty = new EFanty();
	this->_fanty->setTileSize(VECTOR2(32, 32));
	this->_fanty->setChaseable(this);

	this->_uwolAnimPlayer = new AnimationPlayer();
	this->_uwolAnimPlayer->setAnimation(Animation::Get("uwol_walk_right"));
	this->_uwolX = -32;
	this->_uwolVX = UWOL_VX;
}

Credits::~Credits(void)
{
	this->Dispose();
}

void Credits::OnEnter(void)
{
	_currentPauseIdx = 0;
	_pauseTicks = 0;

	this->_fanty->_x = -32;
	this->_fanty->_y = -32;

	this->_uwolX = -32;
	this->_uwolVX = UWOL_VX;

	_g->OffsetY = (_g->ScreenHeight - _g->WorldHeight) / 2.0f;
	this->_tune->PlayAsMusic(true);
}

void Credits::OnExit(void)
{
	_g->OffsetY = (_g->ScreenHeight - _g->WorldHeight) / 2.0f;
	MusicManager::FadeOutMusic(300);
}

string Credits::Update(Uint32 milliSec, Event & inputEvent)
{
	if (_pauseTicks < 5000) {
		this->_pauseTicks += milliSec;
	}
	else {
		_g->OffsetY -= (milliSec / 25.0f);
		if (_g->OffsetY <= -this->_textPauseY[_currentPauseIdx]) {
			_pauseTicks = 0;
			_currentPauseIdx++;
		}
	}

	_uwolX += _uwolVX;

	if (_uwolX < -64 && _uwolVX < 0) {
		_uwolVX = UWOL_VX;
	}
	if (_uwolX > (_g->ScreenWidth + 32) && _uwolVX > 0) {
		_uwolVX = -UWOL_VX;
	}
	_uwolAnimPlayer->Update(milliSec);

	if (inputEvent.Name == "KEY_UP" || inputEvent.Name == "JOY_UP" || _currentPauseIdx >= this->_textPauseY.size()) {
		return "Portada";
	}

	this->_fanty->Update(milliSec, inputEvent);

	return this->Name;
}

void Credits::Draw(void)
{
	int posY = 32;
	int ofsY = (_g->ScreenHeight - _g->WorldHeight) / 2;
	int botY = _g->ScreenHeight - ofsY;
	int bandHeight = 4;

	this->_textPauseY.clear();

	this->_fanty->Draw();

	drawFrame(&posY, _uwolIcon); posY += 32;
	
	this->_textPauseY.push_back((float) posY - ofsY - 32);
	drawCentered(posY, "^Designed by _Anjuel ^& _Na-th-an"); posY += 16;
	drawCentered(posY, "^Story by _David Pimperl Marco"); posY += 16;
	drawCentered(posY, "^Code by _Na-th-an"); posY += 16;
	drawCentered(posY, "^Ingame graphics by _Anjuel"); posY += 16;
	drawCentered(posY, "^Additional gfx. by _Na-th-an"); posY += 16;
	drawCentered(posY, "^Cover art by _Ferran Criado"); posY += 16;
	drawCentered(posY, "^Music by _Augusto Ruiz"); posY += 16;
	drawCentered(posY, "^Development tools by _Augusto Ruiz"); posY += 16;

	drawCentered(posY, "^Levels & supp.: _Augus, Anjuel,"); posY += 16;
	drawCentered(posY, "_Na-th-an, Kendroock, Daivod,"); posY += 16;
	drawCentered(posY, "_Dadman, T.Brazil, IvanZX,"); posY += 16;
	drawCentered(posY, "_Benway, Sejuan, Konamito,"); posY += 16;
	drawCentered(posY, "_Danthalas, Metalbrain, Zemman"); posY += 64;

	drawCentered(posY, "*@ 2006 - 2016 The Mojon Twins"); posY += 128;

	this->_textPauseY.push_back((float)posY - ofsY - 48);

	drawCentered(posY, "^UWOL, The Quest For Money"); posY += 16;
	drawCentered(posY, "_PC, Mac, Linux"); posY += 16;
	drawCentered(posY, "_and Raspberry Pi"); posY += 32;

	drawCentered(posY, "^Code and Music: _Augusto Ruiz"); posY += 16;
	drawCentered(posY, "^Sound FX: _WYZ"); posY += 16;
	drawCentered(posY, "^Linux support: _utopian"); posY += 16;
	drawCentered(posY, "^OSX support: _falvarez"); posY += 32;

	drawCentered(posY, "^Testers: _Imanok, Guillian,"); posY += 16;
	drawCentered(posY, "_WYZ, utopian, pagantipaco,"); posY += 16;
	drawCentered(posY, "_Metalbrain, Sejuan, falvarez,"); posY += 16;
	drawCentered(posY, "_David B."); posY += 32;
	
	drawCentered(posY, "*@ 2006 - 2016 Retroworks"); posY += 64;

	this->_textPauseY.push_back((float)posY - ofsY);

	_maxPosY = posY;

	_g->BlitFrameAbs(this->_shadow, 0, 0, _g->ScreenWidth, ofsY, false, false);
	_g->BlitFrameAbs(this->_shadow, 0, botY, _g->ScreenWidth, ofsY, false, false);

	_g->BlitFrameAlphaAbs(_uwolAnimPlayer->GetCurrentFrame(), (int)this->_uwolX, 
		(int)botY, 
		32, 32, 
		1.0, this->_uwolVX < 0, false);

	_g->BlitFrameAlphaAbs(this->_shadow, 0, ofsY, _g->ScreenWidth, bandHeight, 0.75f, false, false);
	_g->BlitFrameAlphaAbs(this->_shadow, 0, ofsY + bandHeight, _g->ScreenWidth, bandHeight, 0.5f, false, false);
	_g->BlitFrameAlphaAbs(this->_shadow, 0, ofsY + (2* bandHeight), _g->ScreenWidth, bandHeight, 0.25f, false, false);

	_g->BlitFrameAlphaAbs(this->_shadow, 0, botY - bandHeight, _g->ScreenWidth, bandHeight, 0.75f, false, false);
	_g->BlitFrameAlphaAbs(this->_shadow, 0, botY - (2* bandHeight), _g->ScreenWidth, bandHeight, 0.5f, false, false);
	_g->BlitFrameAlphaAbs(this->_shadow, 0, botY - (3* bandHeight), _g->ScreenWidth, bandHeight, 0.25f, false, false);

	_g->BlitFrameAbs(this->_shadow, 0, 0, 16, _g->ScreenHeight, false, false);
	_g->BlitFrameAbs(this->_shadow, _g->ScreenWidth - 16, 0, 16, _g->ScreenHeight, false, false);

}

void Credits::drawFrame(int *y, Frame* frame) {
	_g->BlitFrame(frame, (_g->WorldWidth - (frame->Texture->width * 2)) / 2, *y, frame->Texture->width * 2, frame->Texture->height * 2, false, false);
	*y += (frame->Texture->height * 2) + 32;
}

bool Credits::ShouldChase() {
	return true;
}

VECTOR2F Credits::GetPosition() {
	return VECTOR2F(_uwolX - _g->OffsetX, _g->ScreenHeight - (_g->ScreenHeight - _g->WorldHeight) / 2 - _g->OffsetY - 64);
}

Program* Credits::GetProgram(void) {
	return nullptr;
}

void Credits::drawCentered(int y, const string &str) {
	string strCopy(str);

	strCopy.erase(std::remove(strCopy.begin(), strCopy.end(), '^'), strCopy.end());
	strCopy.erase(std::remove(strCopy.begin(), strCopy.end(), '_'), strCopy.end());

	int strWidth = (strCopy.size() * _textSize);
	int posX = (_g->WorldWidth - strWidth) / 2;
	float rT = _rTitleTop, gT = _gTitleTop, bT = _bTitleTop;
	float rB = _rTitleBot, gB = _gTitleBot, bB = _bTitleBot;

	for (char c : str) {
		switch (c) {
		case '^':
			rT = _rTitleTop, gT = _gTitleTop, bT = _bTitleTop;
			rB = _rTitleBot, gB = _gTitleBot, bB = _bTitleBot;
			break;
		case '_':
			rT = _rTextTop, gT = _gTextTop, bT = _bTextTop;
			rB = _rTextBot, gB = _gTextBot, bB = _bTextBot;
			break;
		case '*':
			rT = _rGoldTop, gT = _gGoldTop, bT = _bGoldTop;
			rB = _rGoldBot, gB = _gGoldBot, bB = _bGoldBot;
			break;
		default:
			string character(1, c);
			//_g->DrawStringAlpha(posX + 2, y + 2, _textSize, character, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.3f);
			_g->DrawString(posX, y, _textSize, character, rT, gT, bT, rB, gB, bB);
			posX += _textSize;
			break;
		}
	}
}

void Credits::Dispose(void) {
	if (!this->_disposed) {
		this->_disposed = true;

		delete this->_uwolIcon;
		delete this->_tune;
		delete this->_shadow;
		delete this->_fanty;

		delete this->_uwolAnimPlayer;
	}
}
