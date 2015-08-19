#include "Presentacion.h"

#define NUM_COINS 300
#define INCR_FACTOR 1

Presentacion::Presentacion() {
	this->Name = "Presentacion";
	VECTOR2 tileSize;

	tileSize.x = 32;
	tileSize.y = 32;

	this->_g = Graphics::GetInstance();

	this->_frameSombra = Frame("data/TileSombra.PNG");
	this->_logos.push_back(Frame("data/texturaUWOL.png"));

	this->_disposed = false;

	this->_coins = new Coin*[NUM_COINS];

	for (int i = 0; i < NUM_COINS; i++)
	{
		this->_coins[i] = new Coin();
		this->_coins[i]->setTileSize(tileSize);
		this->_coins[i]->_x = (int)((_g->ScreenWidth)*rand() / (RAND_MAX + 1.0));
		this->_coins[i]->_y = (int)((_g->ScreenHeight)*rand() / (RAND_MAX + 1.0));
		this->_coins[i]->SetFrame((int)(7 * rand() / (RAND_MAX + 1.0)));
		this->_coins[i]->_speed.y = (i/100) + 1 + (float)(2 * rand() / (RAND_MAX + 1.0));
		this->_coins[i]->SetTicks((int)(TICKS_ANIM_COIN * rand() / (RAND_MAX + 1.0)));
		this->_coins[i]->_rotationFactor = (float)((3 * rand() / (RAND_MAX + 1.0)) + 0.5f);
	}
}

Presentacion::~Presentacion() {
	this->Dispose();
}

void Presentacion::OnEnter() {
	this->_currentAlpha = 0.0f;
	this->_currentTick = 0;
	this->_totalTicks = 0;
	this->_currentFrame = this->_logos.begin();

	this->_incrFactor = INCR_FACTOR;

	MusicManager::PlayMusic("music/Money.ogg", true);
}

void Presentacion::OnExit() {
	MusicManager::FadeOutMusic(300);
}

void Presentacion::Dispose()
{
	if (!this->_disposed) {
		for (int i = 0; i < NUM_COINS; i++) {
			delete this->_coins[i];
		}
		delete this->_coins;
		this->_disposed = true;
	}
}

void Presentacion::Draw() {
	Frame &current = *(this->_currentFrame);

	for (int i = 0; i < NUM_COINS; i++) {
		this->_coins[i]->DrawInPos(this->_coins[i]->_x, this->_coins[i]->_y, 1.0f);
	}

	_g->BlitCenteredFrameAlpha(current, current.Texture->width * 2, current.Texture->height * 2, this->_currentAlpha, false, false);

	string text = "PUSH JUMP TO START";
	int posX = (_g->WorldWidth - (text.size() * 16)) / 2;
	int posY = _g->WorldHeight - 32;

	_g->DrawStringAlpha(posX + 1, posY + 1, text, 0, 0, 0, 0, 0, 0, _textAlpha);
	_g->DrawStringAlpha(posX, posY, text, _rTextTop, _gTextTop, _bTextTop, _rTextBot, _gTextBot, _bTextBot, _textAlpha);

	_g->BlitFrameAbs(this->_frameSombra, 0, 0, _g->ScreenWidth, _g->OffsetY - 1, false, false);
	_g->BlitFrameAbs(this->_frameSombra, 0, _g->ScreenHeight - _g->OffsetY, _g->ScreenWidth, _g->OffsetY, false, false);
	_g->BlitFrameAbs(this->_frameSombra, 0, 0, _g->OffsetX, _g->ScreenHeight, false, false);
	_g->BlitFrameAbs(this->_frameSombra, _g->ScreenWidth - _g->OffsetX, 0, _g->OffsetX, _g->ScreenHeight, false, false);
}

string Presentacion::Update(Uint32 milliSec, Event & inputEvent) {
	float val = (float)(.01 * this->_totalTicks);
	this->_rTextTop = (float)(sin(val + 0) * .5 + .5);
	this->_gTextTop = (float)(sin(val + 2) * .5 + .5);
	this->_bTextTop = (float)(sin(val + 4) * .5 + .5);

	this->_totalTicks += milliSec;

	val = (float)(.017 * this->_totalTicks);
	this->_rTextBot = (float)(sin(val + 0) * .5 + .5);
	this->_gTextBot = (float)(sin(val + 2) * .5 + .5);
	this->_bTextBot = (float)(sin(val + 4) * .5 + .5);

	this->_textAlpha = 1.0f; //(float) (sin(.05 * (this->_totalTicks/milliSec)) * .5 + .5);

	this->_currentAlpha += ((float)this->_incrFactor) *  milliSec * 0.001f;

	if (this->_incrFactor == 0) {
		this->_currentTick += milliSec;

		if (this->_currentTick >= 60000) {
			this->_currentTick = 0;
			this->_incrFactor = -INCR_FACTOR;
		}
	}

	if (this->_currentAlpha > 1.0f) {
		this->_incrFactor = 0;
		this->_currentAlpha = 1.0f;
	}

	if (this->_currentAlpha < 0.0f && milliSec > 0) {
		this->_incrFactor = INCR_FACTOR;
		this->_currentAlpha = 0.0f;
		this->UpdateCurrentTexture();
	}

	for (int i = 0; i<NUM_COINS; i++) {
		this->_coins[i]->Update(milliSec);
		this->_coins[i]->_y += (int) this->_coins[i]->_speed.y;
		if (this->_coins[i]->_y > _g->ScreenHeight) {
			this->_coins[i]->_y = 0;
			this->_coins[i]->_x = (int)((_g->ScreenWidth)*rand() / (RAND_MAX + 1.0));
			this->_coins[i]->SetFrame((int)(7 * rand() / (RAND_MAX + 1.0)));
			this->_coins[i]->_speed.y = (i / 100) + 1 + (float)(2 * rand() / (RAND_MAX + 1.0));
			this->_coins[i]->SetTicks((int)(TICKS_ANIM_COIN * rand() / (RAND_MAX + 1.0)));
		}
	}

	InputManager *input = InputManager::GetInstance();
	if (inputEvent.Name == "KEY_DOWN") {
		ActionKeys key = (ActionKeys)inputEvent.Data["key"].asInt();
		if (key == ActionKeysUp) {
			input->SetControlMode(Keyboard);
			return "Piramide";
		}
	}
	if (inputEvent.Name == "JOY_DOWN") {
		int button = inputEvent.Data["button"].asInt();
		if (button == JOY_BUTTON_0 || button == JOY_BUTTON_1) {
			input->SetControlMode(Joystick);
			return "Piramide";
		}
	}

	if (this->_currentFrame != this->_logos.end()) {
		return this->Name;
	}
	else {
		return "Portada";
	}
}

void Presentacion::UpdateCurrentTexture()
{
	this->_currentFrame++;
}