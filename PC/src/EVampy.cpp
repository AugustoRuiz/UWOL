#include "EVampy.h"

EVampy::EVampy() : Enemigo()
{
	this->Initialize();
}

EVampy::~EVampy()
{
}

void EVampy::Initialize()
{
	this->_animPlayer.setAnimation(Animation::Get("vampy_float"));
	Enemigo::Initialize();
	this->setTipoEnemigo(Vampy);
}