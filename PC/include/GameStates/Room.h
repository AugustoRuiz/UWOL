#pragma once

#include <fstream>
#include "GameData.h"
#include "CollisionMap.h"
#include "Background.h"
#include "Plataforma.h"
#include "Enemigo.h"
#include "FactoriaEnemigo.h"
#include "Coin.h"
#include "Player.h"
#include <cmath>

class Room : public IGameState
{
public:
	Room(void);
	~Room(void);

	// Implementación de IGameState.
	virtual void OnEnter(void) override;
	virtual void OnExit(void) override;
	virtual string Update(Uint32 milliSec, Event & inputEvent) override;
	virtual void Draw(void) override;
	virtual void Dispose(void) override;
	virtual Program * GetProgram(void) override;

	void Restart();

	void setTileFondo(TilesFondo tile);
	void setPlayer(TPlayer *player);

	int TimeLeft;
	bool CheckTime;
	bool Completada;
	void SetDepth(int value);
	int GetDepth();

	Plataforma* AddPlatform(TilePlataforma tipo, Direccion dir, char longitud, char tileX, char tileY, VECTOR2 tileSize);
    Enemigo* AddEnemy(TipoEnemigo tipo, Velocidad velocidad, char tileIni, char tileFin, char tileVert, VECTOR2 tileSize);
    Coin* AddCoin(char tileX, char tileY, VECTOR2 tileSize);
        
	TPlayer *_player;

	bool loadRoom(istream *roomsFile);

	int getEstado();

	unsigned int GetRand();

	static void StaticInit();
	static void StaticDispose();

private:
	int _depth;
	unsigned int _rand;
	bool _disposed;

	int _initialTime;
	int _estado;

	float _valorOscuro, _ticks, _sinValue;

	Frame* _texFlechaIzq, *_texFlechaDer;

	static Frame* _textureCamiseta;
	static Frame* _texturePuerta;
	static Frame* _textureFlechaIzq;
	static Frame* _textureFlechaDer;
	static Frame* _textureSombra;
	static Frame* _textureFlecha;
	static Sound* _fxCoin, *_fxHit, *_fxGhost, *_musicGhost, *_fxCamiseta;
	static vector<Sound*> _tunes;
	Sound* _tune;

	Background *_back;

	Graphics *_g;

	CollisionMap *_map;

	Enemigo* _fanty;

	//void AddFanty();

	void pickCoins(const RECTANGLEF& rect);
	void checkEnemies(const RECTANGLEF& rect);
	void pickCamiseta(const RECTANGLEF& rect);

	void colocarCamiseta();
	void quitarCamiseta();

	void drawSombraCamiseta();
	void drawCamiseta();

	int _camisetaX, _camisetaY;
	bool _camisetaVisible;

	std::vector<IUpdatable*> _updatables;
	std::vector<IDrawable*> _drawables;

    std::vector<Plataforma*> _plataformas;
    std::vector<Enemigo*> _enemigos;
	std::vector<Coin*> _monedasRecogidas;
    std::vector<Coin*> _monedas;
};
