#pragma once
#include "Enemigo.h"

class EFranky : public Enemigo
{
public:
	EFranky();
	~EFranky();

	void Initialize();
	void Update(Uint32 milliSec);
	void Draw();
	void DrawShadow(void);

protected:
	static const char ESTADO_MOVIENDO = 0;
	static const char ESTADO_GIRANDO = 1;

	char _estado;

	void updateCurrentTexture();
};