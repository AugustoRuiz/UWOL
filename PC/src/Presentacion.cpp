#include "Presentacion.h"

#define NUM_COINS 500
#define INCR_FACTOR 1

Presentacion::Presentacion()
{
	this->Initialize();
}

Presentacion::~Presentacion()
{
	this->Dispose();
}

void Presentacion::Initialize()
{
	this->Name = "Presentacion";
	VECTOR2 tileSize;

	tileSize.x = 32;
	tileSize.y = 32;

	TextureMgr *texMgr = TextureMgr::GetInstance();

	this->_g = Graphics::GetInstance();

	this->_frameSombra = Frame("data/TileSombra.PNG");

	this->_logos.push_back(Frame("data/texturaUWOL.png"));
	this->_currentFrame = this->_logos.begin();

	this->_disposed = false;

	this->_currentAlpha = 0.0f;
	this->_currentTick = 0;

	this->_incrFactor = INCR_FACTOR;

	this->_coins = new Coin*[NUM_COINS];

	for (int i = 0; i < NUM_COINS; i++)
	{
		this->_coins[i] = new Coin();
		this->_coins[i]->setTileSize(tileSize);
		this->_coins[i]->_x = (int)((_g->ScreenWidth)*rand() / (RAND_MAX + 1.0));
		this->_coins[i]->_y = (int)((_g->ScreenHeight)*rand() / (RAND_MAX + 1.0));
		this->_coins[i]->SetFrame((int)(7 * rand() / (RAND_MAX + 1.0)));
		this->_coins[i]->_speed.y = 1 + (float)(10 * rand() / (RAND_MAX + 1.0));
		this->_coins[i]->SetTicks((int)(TICKS_ANIM_COIN * rand() / (RAND_MAX + 1.0)));
	}
}

void Presentacion::Dispose()
{
	if (!this->_disposed)
	{
		for (int i = 0; i < NUM_COINS; i++)
		{
			delete this->_coins[i];
		}
		delete this->_coins;
		this->_disposed = true;
	}
}

void Presentacion::Draw()
{
	Frame &current = *(this->_currentFrame);

	for (int i = 0; i < NUM_COINS; i++) {
		this->_coins[i]->DrawInPos(this->_coins[i]->_x, this->_coins[i]->_y, 1.0f);
	}

	_g->BlitCenteredFrameAlpha(current, current.Texture->width * 2, current.Texture->height * 2, this->_currentAlpha, false, false);

	_g->BlitFrameAbs(this->_frameSombra, 0, 0, _g->ScreenWidth, _g->OffsetY - 1, false, false);
	_g->BlitFrameAbs(this->_frameSombra, 0, _g->ScreenHeight - _g->OffsetY, _g->ScreenWidth, _g->OffsetY, false, false);
	_g->BlitFrameAbs(this->_frameSombra, 0, 0, _g->OffsetX, _g->ScreenHeight, false, false);
	_g->BlitFrameAbs(this->_frameSombra, _g->ScreenWidth - _g->OffsetX, 0, _g->OffsetX, _g->ScreenHeight, false, false);
}

string Presentacion::Update(Uint32 milliSec, IGameState *lastState)
{
	this->_currentAlpha += ((float)this->_incrFactor) *  milliSec * 0.001f;

	if (this->_incrFactor == 0)
	{
		this->_currentTick += milliSec;

		if (this->_currentTick >= 4000)
		{
			this->_currentTick = 0;
			this->_incrFactor = -INCR_FACTOR;
		}
	}

	if (this->_currentAlpha > 1.0f)
	{
		this->_incrFactor = 0;
		this->_currentAlpha = 1.0f;
	}

	if (this->_currentAlpha < 0.0f && milliSec > 0)
	{
		this->_incrFactor = INCR_FACTOR;
		this->_currentAlpha = 0.0f;
		this->UpdateCurrentTexture();
	}

	for (int i = 0; i<NUM_COINS; i++)
	{
		this->_coins[i]->Update(milliSec);
		this->_coins[i]->_y += (int) this->_coins[i]->_speed.y;
		if (this->_coins[i]->_y > _g->ScreenHeight)
		{
			this->_coins[i]->_y = 0;
			this->_coins[i]->_x = (int)((_g->ScreenWidth)*rand() / (RAND_MAX + 1.0));
			this->_coins[i]->SetFrame((int)(7 * rand() / (RAND_MAX + 1.0)));
			this->_coins[i]->_speed.y = 1 + (float)(10.0f * rand() / (RAND_MAX + 1.0));
			this->_coins[i]->SetTicks((int)(TICKS_ANIM_COIN * rand() / (RAND_MAX + 1.0)));
		}
	}

	if (this->_currentFrame != this->_logos.end())
	{
		return this->Name;
	}
	else
	{
		return "";
	}
}

void Presentacion::UpdateCurrentTexture()
{
	this->_currentFrame++;
}