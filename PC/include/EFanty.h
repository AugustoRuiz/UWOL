#pragma once
#include "Enemigo.h"
#include "math.h"

class EFanty : public Enemigo
{
public:
	EFanty();
	~EFanty();

	virtual bool DrawWhenNoCoins(void) override;
	virtual bool UpdateWhenNoCoins(void) override;

	void Initialize();
	void Update(Uint32 milliSec) override;
	void Draw() override;

private:

	Frame _frameHalo;
	float _haloValue;
};