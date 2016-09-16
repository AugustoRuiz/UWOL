#include "Presentacion.h"

#define NUM_COINS 50

#define INCR_FACTOR 2
#define SONG_LOOP_LENGTH 15400
#define WAIT_TIME_IN_MSECS 2 * SONG_LOOP_LENGTH

Presentacion::Presentacion() {
	this->Name = "Presentacion";
	VECTOR2 tileSize;

	tileSize.x = 32;
	tileSize.y = 32;

	this->_g = Graphics::GetInstance();

	this->_portada = new Frame("data/uwol_portada.png");
	
	vector<string> vertexShaders = { "data/shaders/Default.150.vertex" };
	vector<string> fragmentShaders = { "data/shaders/ColorCycle.150.fragment" };

	Program *prog = new Program(vertexShaders, fragmentShaders);
	if(prog->ProgramId != 0) {
		this->_bg = new Frame("data/warp.png");
		Frame colorCycle("data/warpCycle.png");
		prog->Textures.push_back(this->_bg->Texture);
		prog->Textures.push_back(colorCycle.Texture);
		this->_bg->SetProgram(prog);
	} else {
		this->_bg = new Frame("data/warp_original.png");
	}

	this->_shadow = new Frame("data/TileSombra.png");;

	this->_disposed = false;

	this->_coins = new Coin*[NUM_COINS];

	for (int i = 0; i < NUM_COINS; i++)
	{
		this->_coins[i] = new Coin();
		this->_coins[i]->setTileSize(tileSize);
		this->_coins[i]->_x = (int)_g->OffsetX + (rand() % _g->WorldWidth);
		this->_coins[i]->_y = (float) (rand() % _g->WorldHeight);
		this->_coins[i]->SetFrame(rand() % 7);
		this->_coins[i]->_speed.y = (float) ((i/100.0f) + 2 + (rand() % 200)/100.0f);
		this->_coins[i]->SetTicks(rand() % TICKS_ANIM_COIN);
		this->_coins[i]->_rotationFactor = ((rand() % 30) / 10.0f) + 0.5f;
	}
	this->_music = new Sound("music/Money.ogg");
}

Presentacion::~Presentacion() {
	this->Dispose();
}

void Presentacion::OnEnter() {
	this->_currentAlpha = 0.0f;
	this->_currentTick = 0;
	this->_totalTicks = 0;
	this->_goNext = false;
	this->_incrFactor = INCR_FACTOR;

	this->_music->PlayAsMusic(true);
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
		
		delete this->_bg;
		delete this->_portada;
		delete this->_music;
		delete this->_shadow;

		this->_disposed = true;
	}
}

Program * Presentacion::GetProgram(void)
{
	return nullptr;
}

void Presentacion::Draw() {
	for (int i = 0; i < NUM_COINS/2; i++) {
		this->_coins[i]->DrawInPos(this->_coins[i]->_x, (int) round(this->_coins[i]->_y), 0.5f * this->_currentAlpha);
	}

	int bgW = _bg->Texture->width * 2;
	int bgH = _bg->Texture->height * 2;
	int bgX = (_g->WorldWidth - bgW) / 2;
	int bgY = 41 + (_g->WorldHeight - bgH) / 2;

	Program* prog = _bg->GetProgram();
	if (prog != NULL) {
		prog->SetUniform("iGlobalTime", (this->_currentAlpha < 1.0f ? 0.0f : -(float)this->_totalTicks));
	}
	_g->BlitFrameAlpha(_bg, bgX, bgY, bgW, bgH, this->_currentAlpha, false, false);
	
	int pW = _portada->Texture->width * 2;
	int pH = _portada->Texture->height * 2;
	int x = (_g->WorldWidth - pW) / 2;
	int y = -8 + (_g->WorldHeight - pH) / 2;

	_g->BlitFrameAlpha(_portada, x, y, pW, pH, this->_currentAlpha, false, false);

	string text = "PUSH JUMP TO START";
	int posX = (int)((_g->WorldWidth - (text.size() * 16)) / 2);
	int posY = _g->WorldHeight + 14;

	_g->DrawStringAlpha(posX + 1, posY + 1, text, 0, 0, 0, 0, 0, 0, _textAlpha);
	_g->DrawStringAlpha(posX, posY, text, _rTextTop, _gTextTop, _bTextTop, _rTextBot, _gTextBot, _bTextBot, _textAlpha);

	for (int i = NUM_COINS/2; i < NUM_COINS; i++) {
		this->_coins[i]->DrawInPos(this->_coins[i]->_x, (int)round(this->_coins[i]->_y), 1.0f * this->_currentAlpha);
	}

	// Cortamos por las bravas...
	_g->BlitFrameAbs(this->_shadow, 0, 0, _g->ScreenWidth, (int)_g->OffsetY - 33, false, false);
	_g->BlitFrameAbs(this->_shadow, 0, _g->ScreenHeight - (int)_g->OffsetY + 33, _g->ScreenWidth, (int)_g->OffsetY - 33, false, false);
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

		if (this->_currentTick >= WAIT_TIME_IN_MSECS) {
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
		this->_goNext = true;
	}

	for (int i = 0; i<NUM_COINS; i++) {
		this->_coins[i]->Update(milliSec, inputEvent);
		this->_coins[i]->_y += (int) this->_coins[i]->_speed.y;
		if (this->_coins[i]->_y > _g->ScreenHeight) {
			this->_coins[i]->_y = -32;
			this->_coins[i]->_x = (int)_g->OffsetX + (rand() % _g->WorldWidth);
			this->_coins[i]->SetFrame(rand() % 7);
			this->_coins[i]->_speed.y = (float)((i / 100.0f) + 2 + (rand() % 200) / 100.0f);
			this->_coins[i]->SetTicks(rand() % TICKS_ANIM_COIN);
		}
	}

	InputManager *input = InputManager::GetInstance();
	if (inputEvent.Name == "KEY_UP") {
		ActionKeys key = (ActionKeys)inputEvent.Data["key"].asInt();
		if (key == ActionKeysUp) {
			input->SetControlMode(Keyboard);
			return "Piramide";
		}
	}
	if (inputEvent.Name == "JOY_UP") {
		input->SetControlMode(Joystick);
		return "Piramide";
	}

	if (this->_goNext) {
		return "Attract";
	}

	return this->Name;
}
