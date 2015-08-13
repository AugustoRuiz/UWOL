#pragma once

#include "UWOL.h"
#include "Enemigo.h"
#include "EFanty.h"
#include "EFranky.h"
#include "EVampy.h"
#include "EWolfy.h"

class FactoriaEnemigo
{
public:

	static Enemigo* Create(TipoEnemigo tipo);
};