#include "EFanty.h"

EFanty::EFanty() : Enemigo()
{
	this->Initialize();
}

EFanty::~EFanty()
{
}

void EFanty::Initialize()
{
	this->_haloValue = 0.0f;
	this->_animPlayer.setAnimation(Animation::Get("fanty_float"));
	this->_frameHalo = Frame("data/Halo.png");

	Enemigo::Initialize();

	this->setTipoEnemigo(Fanty);
}

 void EFanty::Draw()
{
	//float alpha = 0.5f + ( sin(_haloValue) / 6.0f );
	float haloAlpha = 0.5f + ( sin(7.0f * _haloValue) / 6.0f );

	_g->BlitColoredFrame(_frameHalo, -16 + (int)this->_x, - 16 + (int) this->_y, this->_tileSize.x * 2, this->_tileSize.y * 2, 1.0f, 1.0f, 1.0f, (1.0f - haloAlpha), true, false, false);

	Enemigo::Draw();
}


bool EFanty::DrawWhenNoCoins()
{
	return true;
}

bool EFanty::UpdateWhenNoCoins()
{
	return true;
}

void EFanty::Update(Uint32 milliSec)
{
	float ptX, ptY;

	_animPlayer.Update(milliSec);

	_haloValue += milliSec * 0.005f;

	if(this->_player->getEstado() & Muriendo)
	{
		ptX = 6.0f * this->_tileSize.x;
		ptY = 5.0f * this->_tileSize.y;
	}
	else
	{
		ptX = this->_player->_x;
		ptY = this->_player->_y;
	}

	if(this->_x < ptX)
	{
		_vx += ACCEL_FANTY * milliSec;
		if(_vx > 3.0f) _vx = 3.0f;
	}
	if(this->_x > ptX)
	{
		_vx -= ACCEL_FANTY * milliSec;
		if(_vx < -3.0f) _vx = -3.0f;
	}
	if(this->_y < ptY)
	{
		_vy += ACCEL_FANTY * milliSec;
		if(_vy > 3.0f) _vy = 3.0f;
	}
	if(this->_y > ptY)
	{
		_vy -= ACCEL_FANTY * milliSec;
		if(_vy < -3.0f) _vy = -3.0f;
	}
	this->_x += PIX_PER_MILLISEC * milliSec * _vx;
	this->_y += PIX_PER_MILLISEC * milliSec * _vy;
	
	if(this->_vx > 0) _direccion = 1;
	if(this->_vx < 0) _direccion = -1;

	_g->LightPosition.x = (int)this->_x;
	_g->LightPosition.y = (int)this->_y;
}
