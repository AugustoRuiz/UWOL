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
	Animation* a = Animation::Get("vampy_float");
	this->_animPlayer.setAnimation(a);
	Enemigo::Initialize();
	this->setTipoEnemigo(Vampy);
}