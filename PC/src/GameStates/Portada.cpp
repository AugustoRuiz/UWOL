#include "Portada.h"

#define INCR_FACTOR 1

Portada::Portada() : _imagenes()
{
	this->Name = "Portada";
	this->_g = Graphics::GetInstance();

	this->_imagenes.push_back(Frame("data/logoMojon.png"));
	//this->_imagenes.push_back(Frame("data/logoRetroworks.png"));

	this->_sonido = Sound("sounds/coinPicked.ogg");

	this->OnEnter();
}

Portada::~Portada()
{
	this->Dispose();
}

void Portada::OnEnter()
{
	this->_currentFrame = 0;

	this->_disposed = false;

	this->_currentAlpha = 1.0f;
	this->_currentTick = 0;

	this->_incrFactor = INCR_FACTOR;
}

void Portada::OnExit() {

}

void Portada::Dispose()
{
	if (!this->_disposed) {
		this->_disposed = true;
	}
}

void Portada::Draw()
{
	Frame current = Frame(this->_imagenes[this->_currentFrame]);
	int w = current.Texture->width * 2;
	int h = current.Texture->height * 2;
	int posX, posY;

	posX = (this->_g->WorldWidth - w) >> 1;
	posY = (this->_g->WorldHeight - h) >> 1;

	_g->BlitColoredFrame(current, posX, posY, w, h,
		this->_currentAlpha, this->_currentAlpha, this->_currentAlpha, this->_currentAlpha, false,
		false, false);
}

string Portada::Update(Uint32 milliSec, Event & inputEvent)
{
	this->_currentAlpha += ((float)this->_incrFactor) *  milliSec * 0.001f;

	if (inputEvent.Name == "KEY_UP" || inputEvent.Name == "JOY_UP") {
		this->_incrFactor = 0;
		this->_currentTick = 0;
		this->_currentAlpha = 1.0f;
		this->UpdateCurrentTexture();
	}
	else {
		if (this->_incrFactor == 0) {
			if (this->_currentTick == 0) {
				this->_sonido.PlayAsFx(false);
			}
			this->_currentTick += milliSec;
			if (this->_currentTick >= WAIT_TIME) {
				this->_currentTick = 0;
				this->_incrFactor = -INCR_FACTOR;
			}
		}

		if (this->_currentAlpha > 1.0f) {
			this->_incrFactor = 0;
			this->_currentAlpha = 1.0f;
		}

		if (this->_currentAlpha < 0.0f && milliSec > 0) {
			this->_incrFactor = 0;
			this->_currentAlpha = 1.0f;
			this->UpdateCurrentTexture();
		}
	}

	if (this->_currentFrame != this->_imagenes.size()) {
		return this->Name;
	}
	else {
		return "Presentacion";
	}
}

void Portada::UpdateCurrentTexture()
{
	this->_currentFrame++;
}

Program * Portada::GetProgram(void)
{
	return nullptr;
}
