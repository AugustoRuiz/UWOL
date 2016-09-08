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
	Animation* frankyFloatAnim = Animation::Get("franky_float_right");
	this->_animPlayer.setAnimation(frankyFloatAnim);

	Enemigo::Initialize();

	this->setTipoEnemigo(Franky);

	this->_estado = EFranky::ESTADO_MOVIENDO;
}

void EFranky::DrawShadow()
{
	Frame* animFrame = _animPlayer.GetCurrentFrame();
		_g->BlitShadow(animFrame, 
			(int)this->_x, (int)this->_y, 
			this->_tileSize.x, this->_tileSize.y, 
			!((this->_estado == EFranky::ESTADO_GIRANDO) || (this->_direccion == 1)), false);
}
 
void EFranky::Draw()
{
	Frame* animFrame = _animPlayer.GetCurrentFrame();
		_g->BlitFrame(animFrame, 
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
			Animation* anim = Animation::Get("franky_turn_right");
			this->_x = (float) (this->_tileIni * this->_tileSize.x);
			this->_direccion *= -1;
			this->_estado = EFranky::ESTADO_GIRANDO;
			this->_animPlayer.setAnimation(anim);
		}

		if(this->_x >= this->_tileFin * this->_tileSize.x)
		{
			Animation* anim = Animation::Get("franky_turn_left");
			this->_x = (float) (this->_tileFin * this->_tileSize.x);
			this->_direccion *= -1;
			this->_estado = EFranky::ESTADO_GIRANDO;
			this->_animPlayer.setAnimation(anim);
		}
	}
	else
	{
		// Esperamos a que termine de girar...
		if (_animPlayer.AnimationEnded) {
			Animation* anim = Animation::Get("franky_float_right");;
			this->_estado = EFranky::ESTADO_MOVIENDO;
			this->_animPlayer.setAnimation(anim);
		}
	}
}