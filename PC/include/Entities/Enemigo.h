#pragma once

#include "GameData.h"
#include "Player.h"

class Enemigo : public IDrawable, public IUpdatable
{
public:
	Enemigo(void);
	virtual ~Enemigo(void);

	void setTipoEnemigo(TipoEnemigo tipo);
	void setTileSize(VECTOR2 tileSize);

	void Draw();
	void DrawShadow(void);
	void DebugPaint(void);

	virtual bool DrawWhenNoCoins(void);
	virtual bool UpdateWhenNoCoins(void);

	void setPosition(char tileIni, char tileFin, char tileVert);
    void resetPosition();
	void setVelocidad(Velocidad vel);
	void setAlpha(float alpha);

	TipoEnemigo getTipoEnemigo();

	int getTileIni();
	int getTileVert();

	RECTANGLEF getCollisionRect();
	void setCollisionRect(const RECTANGLEF& rect);

	void Update(Uint32 milliSec, const Event& inputEvent);

	void Initialize();
	void Dispose();

	float _x;
	float _y;
	
	int _direccion;

protected:
	Graphics *_g;

	AnimationPlayer _animPlayer;

	VECTOR2 _tileSize;
	RECTANGLEF _colRect;

	TipoEnemigo _tipoEnemigo;
	Velocidad _velocidad;

	float _alpha;

	float _vx, _vy;

	char _tileIni;
	char _tileFin;
	char _tileVert;

	bool _disposed;
};
