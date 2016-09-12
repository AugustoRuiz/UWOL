#include "Plataforma.h"

Plataforma::Plataforma(void)
{
	this->Initialize();
}

Plataforma::~Plataforma(void)
{
	this->Dispose();
}

vector<Frame*> Plataforma::_frames;
bool Plataforma::_staticDisposed;

void Plataforma::StaticInit()
{
	_frames.push_back(new Frame("data/TilePlat0.png"));
	_frames.push_back(new Frame("data/TilePlat1.png"));
	_frames.push_back(new Frame("data/TilePlat2.png"));
	_frames.push_back(new Frame("data/TilePlat3.png"));
	_frames.push_back(new Frame("data/TilePlat4.png"));
	_frames.push_back(new Frame("data/TilePlat5.png"));
	_frames.push_back(new Frame("data/TilePlat6.png"));
	_frames.push_back(new Frame("data/TilePlat7.png"));
}

void Plataforma::StaticDispose() {
	if (!_staticDisposed) {
		_staticDisposed = true;
		for (Frame* f : _frames) {
			delete f;
		}
		_frames.clear();
	}
}

void Plataforma::Initialize()
{
	this->_disposed = false;
	this->_currentFrame = NULL;
	this->_g = Graphics::GetInstance();
}

bool Plataforma::DrawWhenNoCoins()
{
	return true;
}

void Plataforma::Dispose()
{
	if(!this->_disposed)
	{
		this->_disposed = true;
		if (this->_currentFrame != NULL) {
			delete this->_currentFrame;
		}
	}
}

void Plataforma::setTipoPlataforma(TilePlataforma tipo)
{
	this->_tipoPlataforma = tipo;
	if (this->_currentFrame != NULL) {
		delete this->_currentFrame;
	}
	this->_currentFrame = new Frame(**(this->_frames.begin() + tipo));
}

void Plataforma::setTileSize(VECTOR2 tileSize)
{
	this->_tileSize = tileSize;
	updateDrawingCoords();
}

void Plataforma::setPos(char tileX, char tileY)
{
	this->_x = tileX;
	this->_y = tileY;
	updateDrawingCoords();
}

void Plataforma::setLongitud(char longitud)
{
	this->_longitud = longitud;
	updateDrawingCoords();
}

void Plataforma::setDireccion(Direccion dir)
{
	this->_direccion = dir;
	updateDrawingCoords();
}

void Plataforma::DrawShadow()
{
	_g->BlitShadow(this->_currentFrame, this->_iniX, this->_iniY, this->_finX, this->_finY, false, false);
}

void Plataforma::Draw()
{
	_g->BlitFrame(this->_currentFrame, _iniX, _iniY, _finX, _finY, false, false);
}

void Plataforma::updateDrawingCoords()
{
	_iniX = this->_x * this->_tileSize.x;
	_iniY = this->_y * this->_tileSize.y;
	_finX = (this->_direccion == Horizontal) ? (this->_longitud * this->_tileSize.x) : this->_tileSize.x;
	_finY = (this->_direccion == Vertical) ? (this->_longitud * this->_tileSize.y) : this->_tileSize.y;
	if (this->_currentFrame != NULL) {
		this->_currentFrame->Coords.tx2 = (this->_direccion == Horizontal) ? (float) this->_longitud : 1.0f;
		this->_currentFrame->Coords.ty2 = (this->_direccion == Vertical) ? (float) this->_longitud : 1.0f;
	}
}
