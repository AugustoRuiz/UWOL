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
	_player = NULL;

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

void Enemigo::setPlayer(TPlayer *player)
{
	this->_player = player;
}

void Enemigo::Update(Uint32 milliSec)
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
