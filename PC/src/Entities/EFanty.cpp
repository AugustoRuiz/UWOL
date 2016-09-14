#include "EFanty.h"

EFanty::EFanty() : Enemigo()
{
	this->Initialize();
}

EFanty::~EFanty()
{
	delete this->_frameHalo;
}

void EFanty::Initialize()
{
	this->_chaseable = NULL;
	this->_haloValue = 0.0f;
	this->_animPlayer.setAnimation(Animation::Get("fanty_float"));
	this->_frameHalo = new Frame("data/Halo.png");

	Enemigo::Initialize();

	this->setTipoEnemigo(Fanty);
}

void EFanty::setChaseable(IChaseable* chaseable) {
	this->_chaseable = chaseable;
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

void EFanty::Update(Uint32 milliSec, const Event& inputEvent)
{
	VECTOR2F target;

	_animPlayer.Update(milliSec);

	_haloValue += milliSec * 0.005f;

	if(this->_chaseable == NULL || !this->_chaseable->ShouldChase())
	{
		target = VECTOR2F(6.0f * this->_tileSize.x, 5.0f * this->_tileSize.y);
	}
	else
	{
		target = this->_chaseable->GetPosition();
	}

	if(this->_x < target.x)
	{
		_vx += ACCEL_FANTY * milliSec;
		if(_vx > 3.0f) _vx = 3.0f;
	}
	if(this->_x > target.x)
	{
		_vx -= ACCEL_FANTY * milliSec;
		if(_vx < -3.0f) _vx = -3.0f;
	}
	if(this->_y < target.y)
	{
		_vy += ACCEL_FANTY * milliSec;
		if(_vy > 3.0f) _vy = 3.0f;
	}
	if(this->_y > target.y)
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
