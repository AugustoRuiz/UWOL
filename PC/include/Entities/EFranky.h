#pragma once
#include "Enemigo.h"

class EFranky : public Enemigo
{
public:
	EFranky();
	~EFranky();

	void Initialize();
	void Update(Uint32 milliSec) override;
	void Draw() override;
	void DrawShadow(void) override;

protected:
	static const char ESTADO_MOVIENDO = 0;
	static const char ESTADO_GIRANDO = 1;

	char _estado;

	void updateCurrentTexture();
};