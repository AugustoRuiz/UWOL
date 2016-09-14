#include "Enemigo.h"

Enemigo::Enemigo()
{
	this->Initialize();
}

Enemigo::~Enemigo()
{
	this->Dispose();
}

void Enemigo::Initialize()
{
	_g = Graphics::GetInstance();

    _alpha = 1.0f;
	_direccion = 1;

	_vx = 0.0f;
	_vy = 0.0f;

	_x = 0.0f;
	_y = 0.0f;

	this->_disposed = false;
}

void Enemigo::Dispose()
{
	if(!this->_disposed)
	{
		this->_disposed = true;
	}
}

bool Enemigo::DrawWhenNoCoins()
{
	return false;
}

bool Enemigo::UpdateWhenNoCoins()
{
	return false;
}

void Enemigo::DrawShadow()
{
	_g->BlitShadow(_animPlayer.GetCurrentFrame(), (int)this->_x, (int)this->_y, this->_tileSize.x, this->_tileSize.y, (this->_direccion != 1), false);
}

void Enemigo::Draw()
{
	_g->BlitFrame(_animPlayer.GetCurrentFrame(), (int)this->_x, (int)this->_y, this->_tileSize.x, this->_tileSize.y, (this->_direccion != 1), false);
}

void Enemigo::setTipoEnemigo(TipoEnemigo tipo)
{
	this->_tipoEnemigo = tipo;
}

TipoEnemigo Enemigo::getTipoEnemigo()
{
	return this->_tipoEnemigo;
}

int Enemigo::getTileIni()
{
	return this->_tileIni;
}

int Enemigo::getTileVert()
{
	return this->_tileVert;
}

void Enemigo::setAlpha(float alpha)
{
    this->_alpha = alpha;
}

void Enemigo::Update(Uint32 milliSec, const Event& inputEvent)
{
	int factor = (this->_velocidad == Rapido) ? 2 : 1;

	this->_x += factor * _direccion * (milliSec * PIX_PER_MILLISEC);

	if(this->_x <= this->_tileIni * this->_tileSize.x)
	{
		this->_x = (float) (this->_tileIni * this->_tileSize.x);
		this->_direccion *= -1;
	}

	if(this->_x >= this->_tileFin * this->_tileSize.x)
	{
		this->_x = (float) (this->_tileFin * this->_tileSize.x);
		this->_direccion *= -1;
	}
	this->_animPlayer.Update(milliSec);
}

void Enemigo::setPosition(char tileIni, char tileFin, char tileVert)
{
	this->_tileIni = tileIni;
	this->_tileFin = tileFin;
	this->_tileVert = tileVert;
    this->resetPosition();
}

void Enemigo::resetPosition() {
    this->_x = (float)(this->_tileIni * this->_tileSize.x) + 1;
    this->_y = (float)(this->_tileVert * this->_tileSize.y);
}

void Enemigo::setVelocidad(Velocidad vel)
{
	this->_velocidad = vel;
}

void Enemigo::setTileSize(VECTOR2 tileSize)
{
	this->_tileSize = tileSize;
}

RECTANGLEF Enemigo::getCollisionRect() {
	RECTANGLEF result;
	result.x = this->_colRect.x + this->_x;
	result.y = this->_colRect.y + this->_y;
	result.width = this->_colRect.width;
	result.height = this->_colRect.height;
	return result;
}

void Enemigo::setCollisionRect(const RECTANGLEF& rect) {
	this->_colRect = rect;
}

void Enemigo::DebugPaint() {
	vector<VECTOR2> vertexes;

	int x1 = (int)(this->_x + this->_colRect.x);
	int x2 = (int)(x1 + this->_colRect.width);
	int y1 = (int)(this->_y + this->_colRect.y);
	int y2 = (int)(y1 + this->_colRect.height);

	vertexes.push_back(VECTOR2(x1, y1));
	vertexes.push_back(VECTOR2(x2, y1));
	vertexes.push_back(VECTOR2(x2, y1));
	vertexes.push_back(VECTOR2(x2, y2));
	vertexes.push_back(VECTOR2(x2, y2));
	vertexes.push_back(VECTOR2(x1, y2));
	vertexes.push_back(VECTOR2(x1, y2));
	vertexes.push_back(VECTOR2(x1, y1));

	_g->DrawPolyLines(vertexes, 1.0f, 1.0f, 1.0f, 1.0f);
}