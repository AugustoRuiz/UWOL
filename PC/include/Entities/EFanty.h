#pragma once
#include "Enemigo.h"
#include "IChaseable.h"
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

	void setChaseable(IChaseable* chaseable);

private:

	Frame _frameHalo;
	float _haloValue;

	IChaseable* _chaseable;
};