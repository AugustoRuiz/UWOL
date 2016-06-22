#include "Credits.h"

#define UWOL_VX 2.5

Credits::Credits(void)
{
	this->Name = "Credits";
	this->_g = Graphics::GetInstance();

	this->_uwolIcon = Frame("data/texturaUWOL.png");
	this->_sdlIcon = Frame("data/SDL_logo.png");
	this->_openGlIcon = Frame("data/Opengl-logo.svg.png");
	this->_freeGlutIcon = Frame("data/freeglut_logo.png");
	this->_glmIcon = Frame("data/glm-logo.png");
	this->_glewIcon = Frame("data/glew-logo.png");

	this->_tune = Sound("music/Ghosts3.ogg");
	this->_shadow = Frame("data/TileSombra.png");

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

	this->_fanty = EFanty();
	this->_fanty.setTileSize(VECTOR2(32, 32));
	this->_fanty.setChaseable(this);

	this->_uwolAnimPlayer.setAnimation(Animation::Get("uwol_walk_right"));
	this->_uwolX = -32;
	this->_uwolVX = UWOL_VX;
}

Credits::~Credits(void)
{
}

void Credits::OnEnter(void)
{
	_totalTicks = 0;
	
	this->_fanty._x = -32;
	this->_fanty._y = -32;

	this->_uwolX = -32;
	this->_uwolVX = UWOL_VX;

	_g->OffsetY = (_g->ScreenHeight - _g->WorldHeight) / 2.0f;
	this->_tune.PlayAsMusic(true);
}

void Credits::OnExit(void)
{
	_g->OffsetY = (_g->ScreenHeight - _g->WorldHeight) / 2.0f;
	MusicManager::FadeOutMusic(300);
}

string Credits::Update(Uint32 milliSec, Event & inputEvent)
{
	_totalTicks += milliSec;
	if (_totalTicks > 5000 ) {
		_g->OffsetY -= (milliSec / 25.0f);
	}

	_uwolX += _uwolVX;

	if (_uwolX < -64 && _uwolVX < 0) {
		_uwolVX = UWOL_VX;
	}
	if (_uwolX > (_g->ScreenWidth + 32) && _uwolVX > 0) {
		_uwolVX = -UWOL_VX;
	}
	_uwolAnimPlayer.Update(milliSec);

	if (inputEvent.Name == "KEY_DOWN" || inputEvent.Name == "JOY_DOWN" || _g->OffsetY < -this->_maxPosY) {
		return "Portada";
	}

	this->_fanty.Update(milliSec);

	return this->Name;
}

void Credits::Draw(void)
{
	int posY = 16;

	this->_fanty.Draw();

	drawFrame(&posY, _uwolIcon);

	drawCentered(posY, "^Designed by _Anjuel ^& _Na-th-an"); posY += 16;
	drawCentered(posY, "^Story by _David Pimperl Marco"); posY += 16;
	drawCentered(posY, "^Code by _Na-th-an"); posY += 16;
	drawCentered(posY, "^Ingame graphics by _Anjuel"); posY += 16;
	drawCentered(posY, "^Additional gfx. by _Na-th-an"); posY += 16;
	drawCentered(posY, "^Music by _Augusto Ruiz"); posY += 16;
	drawCentered(posY, "^Development tools by _Augusto Ruiz"); posY += 32;

	drawCentered(posY, "^Levels & supp.: _Augus, Anjuel,"); posY += 16;
	drawCentered(posY, "_Na-th-an, Kendroock, Daivod,"); posY += 16;
	drawCentered(posY, "_Dadman, T.Brazil, IvanZX,"); posY += 16;
	drawCentered(posY, "_Benway, Sejuan, Konamito,"); posY += 16;
	drawCentered(posY, "_Danthalas, Metalbrain, Zemman"); posY += 32;

	drawCentered(posY, "^Sega Genesis Version By _Shiru"); posY += 16;
	drawCentered(posY, "^PC Version By _Augusto Ruiz"); posY += 32;

	drawCentered(posY, "*@ 2006 - 2016 The Mojon Twins"); posY += 64;

	drawCentered(posY, "^UWOL, The Quest For Money"); posY += 16;
	drawCentered(posY, "_uses the following libraries"); posY += 32;

	drawCentered(posY, "_OpenGL"); posY += 24;
	drawFrame(&posY, _openGlIcon);
	drawCentered(posY, "_SDL 2 - SDL-Image - SDL-Mixer"); posY += 24;
	drawFrame(&posY, _sdlIcon);
	drawCentered(posY, "_glm"); posY += 24;
	drawFrame(&posY, _glmIcon);
	drawCentered(posY, "_glew"); posY += 32;
	drawFrame(&posY, _glewIcon);
	drawCentered(posY, "_freeGlut"); posY += 32;
	drawFrame(&posY, _freeGlutIcon);

	_maxPosY = posY;

	int ofsY = (_g->ScreenHeight - _g->WorldHeight) / 2;
	int botY = _g->ScreenHeight - ofsY;
	int bandHeight = 4;

	_g->BlitFrameAbs(this->_shadow, 0, 0, _g->ScreenWidth, ofsY, false, false);
	_g->BlitFrameAbs(this->_shadow, 0, botY, _g->ScreenWidth, ofsY, false, false);

	_g->BlitFrameAlphaAbs(_uwolAnimPlayer.GetCurrentFrame(), (int)this->_uwolX, 
		(int)botY, 
		32, 32, 
		1.0, this->_uwolVX < 0, false);

	_g->BlitFrameAlphaAbs(this->_shadow, 0, ofsY, _g->ScreenWidth, bandHeight, 0.75f, false, false);
	_g->BlitFrameAlphaAbs(this->_shadow, 0, ofsY + bandHeight, _g->ScreenWidth, bandHeight, 0.5f, false, false);
	_g->BlitFrameAlphaAbs(this->_shadow, 0, ofsY + (2* bandHeight), _g->ScreenWidth, bandHeight, 0.25f, false, false);

	_g->BlitFrameAlphaAbs(this->_shadow, 0, botY - bandHeight, _g->ScreenWidth, bandHeight, 0.75f, false, false);
	_g->BlitFrameAlphaAbs(this->_shadow, 0, botY - (2* bandHeight), _g->ScreenWidth, bandHeight, 0.5f, false, false);
	_g->BlitFrameAlphaAbs(this->_shadow, 0, botY - (3* bandHeight), _g->ScreenWidth, bandHeight, 0.25f, false, false);
}

void Credits::drawFrame(int *y, const Frame& frame) {
	_g->BlitFrame(frame, (_g->WorldWidth - frame.Texture->width) / 2, *y, frame.Texture->width, frame.Texture->height, false, false);
	*y += frame.Texture->height + 32;
}

bool Credits::ShouldChase()
{
	return true;
}

VECTOR2F Credits::GetPosition()
{
	return VECTOR2F(_uwolX - _g->OffsetX, _g->ScreenHeight - (_g->ScreenHeight - _g->WorldHeight) / 2 - _g->OffsetY - 64);
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
			_g->DrawString(posX, y, _textSize, string(1, c), rT, gT, bT, rB, gB, bB);
			posX += _textSize;
			break;
		}
	}
}

void Credits::Dispose(void)
{
	if (!this->_disposed) {
		this->_disposed = true;
	}
}
