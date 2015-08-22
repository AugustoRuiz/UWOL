#pragma once

#include <fstream>
#include "IUpdatable.h"
#include "IDrawable.h"
#include "GameData.h"
#include "IGameState.h"
#include "CollisionMap.h"
#include "Graphics.h"
#include "Background.h"
#include "InputManager.h"
#include "Plataforma.h"
#include "Enemigo.h"
#include "FactoriaEnemigo.h"
#include "Coin.h"
#include "Player.h"
#include <cmath>
#include "MusicManager.h"

class Room : public IGameState
{
public:
	Room(void);
	~Room(void);

	// Implementación de IGameState.
	void OnEnter(void) override;
	void OnExit(void) override;
	string Update(Uint32 milliSec, Event & inputEvent) override;
	void Draw(void) override;
	void Dispose(void) override;

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

	bool loadRoom(ifstream &roomsFile);

	int getEstado();

private:
	int _depth;

	bool _disposed;

	int _initialTime;
	int _estado;

	float _valorOscuro;

	Frame _textureCamiseta;
	Frame _textureFlechaIzq;
	Frame _textureFlechaDer;
	Frame _textureSombra;
	Sound _tune, _fxCoin, _fxHit, _fxGhost, _musicGhost;

	Background *_back;

	Graphics *_g;

	CollisionMap *_map;

	Enemigo* _fanty;

	void AddFanty();

	void pickCoins(int tileX1, int tileX2, int tileY1, int tileY2);
	void checkEnemies(RECTANGLEF rect);

	void pickCamiseta(int tileX1, int tileX2, int tileY1, int tileY2);
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
