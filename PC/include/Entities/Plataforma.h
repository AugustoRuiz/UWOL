#pragma once

#include "GameData.h"

class Plataforma : public IDrawable
{
public:
	Plataforma(void);
	~Plataforma(void);

	void setTipoPlataforma(TilePlataforma tipo);
	void setTileSize(VECTOR2 tileSize);

	void setPos(char tileX, char tileY);
	void setLongitud(char longitud);
	void setDireccion(Direccion dir);

	bool DrawWhenNoCoins();

	void Draw();
	void DrawShadow(void);

	static void StaticInit();
	static void StaticDispose();

	void Initialize();
	void Dispose();

private:

	static vector<Frame*> _frames;
	Frame* _currentFrame;
	VECTOR2 _tileSize;

	Graphics *_g;

	TilePlataforma _tipoPlataforma;
	Direccion _direccion;
	char _x;
	char _y;
	char _longitud;

	int _iniX;
	int _iniY;
	int _finX;
	int _finY;

	bool _disposed;
	static bool _staticDisposed;

	void updateDrawingCoords();
};
