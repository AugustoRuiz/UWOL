#include "FactoriaEnemigo.h"

Enemigo* FactoriaEnemigo::Create(TipoEnemigo tipo)
{
	Enemigo* enem;

	switch(tipo)
	{
		case Fanty:
			enem = new EFanty();
			break;
		case Franky:
			enem = new EFranky();
			break;
		case Vampy:
			enem = new EVampy();
			break;
		case Wolfy:
			enem = new EWolfy();
			break;
		default:
			enem = NULL;
	}

	return enem;
}