#include "EWolfy.h"

EWolfy::EWolfy() : Enemigo()
{
	this->Initialize();
}

EWolfy::~EWolfy()
{
}

void EWolfy::Initialize()
{
	Animation* wolfyFloat = Animation::Get("wolfy_float");
	this->_animPlayer.setAnimation(wolfyFloat);
	Enemigo::Initialize();
	this->setTipoEnemigo(Wolfy);
}