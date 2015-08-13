#include "EFranky.h"

EFranky::EFranky() : Enemigo()
{
	this->Initialize();
}

EFranky::~EFranky()
{
}

void EFranky::Initialize()
{
	this->_animPlayer.setAnimation(Animation::Get("franky_float_right"));

	Enemigo::Initialize();

	this->setTipoEnemigo(Franky);

	this->_estado = EFranky::ESTADO_MOVIENDO;
}

void EFranky::DrawShadow()
{
		_g->BlitShadow(_animPlayer.GetCurrentFrame(), 
			(int)this->_x, (int)this->_y, 
			this->_tileSize.x, this->_tileSize.y, 
			!((this->_estado == EFranky::ESTADO_GIRANDO) || (this->_direccion == 1)), false);
}
 
void EFranky::Draw()
{
		_g->BlitFrame(_animPlayer.GetCurrentFrame(), 
			(int)this->_x, (int)this->_y, this->_tileSize.x, this->_tileSize.y, 
			!((this->_estado == EFranky::ESTADO_GIRANDO) || (this->_direccion == 1)), false);
}


void EFranky::Update(Uint32 milliSec)
{
	this->_animPlayer.Update(milliSec);

	if(this->_estado == EFranky::ESTADO_MOVIENDO)
	{
		int factor = (this->_velocidad == Rapido) ? 2 : 1;
		this->_x += factor * _direccion * (milliSec * PIX_PER_MILLISEC);

		if(this->_x <= this->_tileIni * this->_tileSize.x)
		{
			this->_x = (float) (this->_tileIni * this->_tileSize.x);
			this->_direccion *= -1;
			this->_estado = EFranky::ESTADO_GIRANDO;
			this->_animPlayer.setAnimation(Animation::Get("franky_turn_right"));
		}

		if(this->_x >= this->_tileFin * this->_tileSize.x)
		{
			this->_x = (float) (this->_tileFin * this->_tileSize.x);
			this->_direccion *= -1;
			this->_estado = EFranky::ESTADO_GIRANDO;
			this->_animPlayer.setAnimation(Animation::Get("franky_turn_left"));
		}
	}
	else
	{
		// Esperamos a que termine de girar...
		if (_animPlayer.AnimationEnded) {
			this->_estado = EFranky::ESTADO_MOVIENDO;
			if (this->_direccion == 1) {
				this->_animPlayer.setAnimation(Animation::Get("franky_float_right"));
			}
			else {
				this->_animPlayer.setAnimation(Animation::Get("franky_float_right"));
			}
		}
	}
}