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
	this->_animPlayer.setAnimation(Animation::Get("wolfy_float"));
	Enemigo::Initialize();
	this->setTipoEnemigo(Wolfy);
}