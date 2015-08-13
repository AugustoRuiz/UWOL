#include "Portada.h"

#define INCR_FACTOR 1

Portada::Portada()
{
	this->Initialize();
}

Portada::~Portada()
{
	this->Dispose();
}

void Portada::Initialize()
{
	this->Name = "Portada";
	this->_g = Graphics::GetInstance();
	
	this->_imagenes.push_back(Frame("data/logoMojon.PNG"));
	this->_imagenes.push_back(Frame("data/logoRetroworks.PNG"));
	this->_imagenes.push_back(Frame("data/uwol_portada.PNG"));
	this->_imagenes.push_back(Frame("data/credits.PNG"));

	this->_currentFrame = this->_imagenes.begin();

	this->_disposed = false;

	this->_currentAlpha = 0.0f;
	this->_currentTick = 0;

	this->_incrFactor = INCR_FACTOR;
}

void Portada::Dispose()
{
	if(!this->_disposed)
	{
		this->_disposed = true;
	}
}

void Portada::Draw()
{
	Frame& current = *this->_currentFrame;
	int w = current.Texture->width * 2;
	int h = current.Texture->height * 2;
	int posX, posY;

	posX = (this->_g->WorldWidth - w) >> 1;
    posY = (this->_g->WorldHeight - h) >> 1;

	_g->BlitColoredFrame(current, posX, posY, w, h,
		this->_currentAlpha, this->_currentAlpha, 1.0f, this->_currentAlpha, false, 
		false, false);
}

string Portada::Update(Uint32 milliSec, IGameState *lastState)
{
	this->_currentAlpha += ((float)this->_incrFactor) *  milliSec * 0.001f;

	if(this->_incrFactor == 0)
	{
		this->_currentTick += milliSec;

		if(this->_currentTick >= 4000)
		{
			this->_currentTick = 0;
			this->_incrFactor = - INCR_FACTOR;
		}
	}

	if(this->_currentAlpha > 1.0f)
	{
		this->_incrFactor = 0;
		this->_currentAlpha = 1.0f;
	}

	if(this->_currentAlpha < 0.0f && milliSec > 0)
	{
		this->_incrFactor = INCR_FACTOR;
		this->_currentAlpha = 0.0f;
		this->UpdateCurrentTexture();
	}

	if(this->_currentFrame != this->_imagenes.end())
	{
		return this->Name;
	}
	else
	{
		return "";
	}
}

void Portada::UpdateCurrentTexture()
{
	this->_currentFrame++;
}