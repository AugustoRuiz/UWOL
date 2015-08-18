#include "Room.h"

Room::Room(void) {
	this->Name = "Room";
	_g = Graphics::GetInstance();

	_back = new Background();

	VECTOR2 vect;

	vect.x = 32;
	vect.y = 32;

	this->_map = new CollisionMap();
	this->_map->Initialize(MAP_COLS, MAP_ROWS, vect.x, vect.y);

	this->_checkTime = true;

	this->_textureFlechaDer = Frame("data/FlechaDer.png");
	this->_textureFlechaIzq = Frame("data/FlechaIzq.png");
	this->_textureCamiseta = Frame("data/Camiseta.png");
	this->_textureSombra = Frame("data/pixBlanco.png");

	this->_disposed = false;

	this->Completada = false;
}

Room::~Room(void) {
	this->Dispose();
}

void Room::OnEnter(void) {
	this->_camisetaVisible = false;

	if (!this->Completada) {
		vector<Coin*>::iterator iter;
		Coin *coin;
		for (iter = this->_monedasRecogidas.begin(); iter != this->_monedasRecogidas.end(); iter++)
		{
			coin = *iter;
			this->_monedas.push_back(coin);
			this->_drawables.push_back(coin);
		}
		this->_monedasRecogidas.clear();
	}

	this->restartLevel();

	string song;
	if (this->Depth < 4)
		song = "music/Zona1.ogg";
	else if (this->Depth < 7)
		song = "music/Zona2.ogg";
	else if (this->Depth < 9)
		song = "music/Zona3.ogg";
	else
		song = "music/Zona4.ogg";

	MusicManager::PlayMusic(song, true);
}

void Room::OnExit() {
	MusicManager::FadeOutMusic(300);
}

void Room::setPlayer(Player *player) {
	if(player != NULL) {
		VECTOR2 vect;
		vect.x = 32;
		vect.y = 32;

		player->setTileSize(vect);
		player->setMap(this->_map);
	}

	if(player != _player) {
		this->_updatables.push_back(player);
		_player = player;
	}
}

string Room::Update(Uint32 milliSec, Event & inputEvent) {
	bool playerWasKilled = false;

	if(this->_player->getEstado() & Muriendo) {
		playerWasKilled = true;
		this->_checkTime = false;
	}

	for (vector<IUpdatable*>::iterator it = _updatables.begin(); it != _updatables.end();++it) {
		IUpdatable* current = *it;
		if(!(this->_player->getEstado() & (TodasMonedasCogidas | Muriendo)) || (current->UpdateWhenNoCoins())) {
			current->Update(milliSec);
		}
	}

	if(!(this->_player->getEstado() & Muriendo) && playerWasKilled) {
		this->OnExit();
		return "Piramide";
	}

	int tileX1, tileX2, tileY1, tileY2;
	this->_player->getTiles(tileX1, tileY1, tileX2, tileY2);

	if(!(this->_player->getEstado() & Muriendo)) {
		this->pickCoins(tileX1, tileX2, tileY1, tileY2);
		this->pickCamiseta(tileX1, tileX2, tileY1, tileY2);

		if (!(this->_player->getEstado() & Parpadeo)) {
			this->checkEnemies(this->_player->_posRect);
		}
	}

	if(this->_checkTime) {
		this->_timeLeft -= milliSec;
		if(this->_timeLeft <= 0) {
			this->_timeLeft = 0;
			this->_checkTime = false;
			this->AddFanty();
			
			MusicManager::FadeOutMusic(500);
			MusicManager::PlayFx("sounds/Bells.ogg", false);
		}
	}
	else
	{
		if(!MusicManager::IsPlayingMusic()) {
			MusicManager::PlayMusic("music/Ghosts3.ogg", true);
		}
	}

	if(this->_timeLeft <= 0 && _valorOscuro < 0.5f) {
		_valorOscuro += milliSec * 0.001f * 0.4f;
	}

	this->_estado = this->_player->getEstado();

	return this->Name;
}

int Room::getEstado() {
	return this->_estado;
}

void Room::restartLevel() {
	vector<Enemigo*>::iterator enemIter;
	vector<IDrawable*>::iterator iterDraw;	
	Enemigo *enem;

	// Posición del foco!
	_g->LightPosition.x = _g->WorldWidth / 2;
	_g->LightPosition.y = 0;
	_valorOscuro = 0.0001f;

	this->_timeLeft = this->Completada ? 0 : this->_initialTime;
	this->_checkTime = true;

	this->_player->initializePlayerData();
	this->quitarCamiseta();

	for(enemIter = this->_enemigos.begin(); enemIter != this->_enemigos.end(); enemIter++) {
		enem = *enemIter;
		if(enem->getTipoEnemigo() == Fanty) {
			// Borro el enemigo...
			this->_enemigos.erase(enemIter);

			// Lo elimino de la lista de updatables y drawables.
			std::vector<IDrawable*>::iterator drwIter;
			std::vector<IUpdatable*>::iterator updIter;
			for(updIter = this->_updatables.begin(); updIter != this->_updatables.end(); updIter++) {
				if(*updIter == enem) {
					this->_updatables.erase(updIter);
					break;
				}
			}
			for(drwIter = this->_drawables.begin(); drwIter != this->_drawables.end(); drwIter++) {
				if(*drwIter == enem) {
					this->_drawables.erase(drwIter);
					break;
				}
			}
			break;
		}
	}
}

void Room::checkEnemies(RECTANGLEF rect) {
	vector<Enemigo*>::iterator iter;
	Enemigo* enem;

	for(iter = this->_enemigos.begin() ; iter != this->_enemigos.end(); iter++) {
		enem = *iter;

		if(enem->getTipoEnemigo() == Fanty || !(this->_player->getEstado() & (TodasMonedasCogidas | Muriendo))) {
			// Rectangle intersection: r1->player, r2->enemy.
			//!( r2->left > r1->right || r2->right < r1->left || r2->top > r1->bottom || r2->bottom < r1->top)
			if(!(enem->_x > rect.x + rect.width - 1 || enem->_x + this->_map->cellWidth - 1 < rect.x ||
				 enem->_y > rect.y + rect.height - 1 || enem->_y + this->_map->cellHeight - 1 < rect.y)) {
				if(this->_player->getEstado() & Desnudo) {
					this->_player->setEstado(Muriendo);
				}

				if(this->_player->getEstado() & Normal)	{
					this->_player->setEstado((this->_player->getEstado() & 0xFFFFFFF0) | Parpadeo | Desnudo);
					this->colocarCamiseta();
					MusicManager::PlayFx("sounds/Hu.ogg", false);
				}
				enem->_direccion *= -1;
			}
		}
	}
}

void Room::pickCamiseta(int tileX1, int tileX2, int tileY1, int tileY2) {
	if(
	   (tileX1 == _camisetaX || tileX2 == _camisetaX)
	   &&
	   (tileY1 == _camisetaY || tileY2 == _camisetaY)
	   &&
	   (this->_player->getEstado() & Desnudo)
	  ) {
		this->quitarCamiseta();
		this->_player->_score += 5;
		this->_player->setEstado((this->_player->getEstado() & 0xFFFFFFF0) | Normal);
	}
}

void Room::colocarCamiseta() {
	bool posCorrecta = false;
	
	do {
		this->_camisetaX = (int)(((double) rand() / (double) RAND_MAX) * 11 + 0);
		this->_camisetaY = (int)(((double) rand() / (double) RAND_MAX) * 9 + 0);

		posCorrecta = (this->_map->map[this->_camisetaY * this->_map->cols + this->_camisetaX] != COLLISION_BLOCK);
		
		if(posCorrecta) {
			// Comprobamos que no colisione con ninguna moneda.
			vector<Coin*>::iterator iter;
			Coin *coin;
			
			for(iter = this->_monedas.begin(); iter != this->_monedas.end(); iter++) {
				coin = *iter;
				if( this->_camisetaX == coin->_x && this->_camisetaY == coin->_y) {
					posCorrecta = false;
					break;
				}
			}

			if(posCorrecta)	{
				vector<Enemigo*>::iterator iterEnem;
				Enemigo *enem;
				
				for(iterEnem = this->_enemigos.begin(); iterEnem != this->_enemigos.end(); iterEnem++) {
					enem = *iterEnem;
					if( this->_camisetaX == enem->getTileIni() && this->_camisetaY == enem->getTileVert()) {
						posCorrecta = false;
						break;
					}
				}
			}
		}

		if(posCorrecta)	{
		   this->_camisetaVisible = true;
		}

	} 
	while(!this->_camisetaVisible);
}

void Room::quitarCamiseta() {
	this->_camisetaVisible = false;
}


void Room::pickCoins(int tileX1, int tileX2, int tileY1, int tileY2) {
	vector<Coin*>::iterator iter;
	vector<IDrawable*>::iterator iterDraw;
	Coin *coin;
	
	for(iter = this->_monedas.begin(); iter != this->_monedas.end(); iter++) {
		coin = *iter;
		if( (tileX1 == coin->_x || tileX2 == coin->_x)
			&&
			(tileY1 == coin->_y || tileY2 == coin->_y)
		  )	{
			for(iterDraw = this->_drawables.begin(); iterDraw != this->_drawables.end(); iterDraw++) {
				if(*iterDraw == coin) {
					this->_drawables.erase(iterDraw);
					break;
				}
			}
			this->_monedas.erase(iter);
			this->_monedasRecogidas.push_back(coin);

			this->_player->_coinsTaken ++;
			this->_player->_score ++;
			
			MusicManager::PlayFx("sounds/coinPicked.ogg", false);
			break;
		}
	}

	if(this->Completada || (this->_monedas.size() == 0 && !(this->_player->getEstado() & (TodasMonedasCogidas | Muriendo)))) {
		this->_player->setEstado(this->_player->getEstado() | TodasMonedasCogidas);
	}
}

void Room::Draw(void) {
	int count = (int) this->_drawables.size();

	_back->Draw();

	for(int ii=0; ii < count; ii++) {
		IDrawable* current = _drawables[ii];
		if(!(this->_player->getEstado() & (TodasMonedasCogidas | Muriendo)) || (current->DrawWhenNoCoins())) {
			current->DrawShadow();
		}
	}
	if(this->_camisetaVisible) {
		this->drawSombraCamiseta();
	}

	_player->DrawShadow();

	for(int ii=0; ii < count; ii++) {
		IDrawable* current = _drawables[ii];
		if(!(this->_player->getEstado() & (TodasMonedasCogidas | Muriendo)) || (current->DrawWhenNoCoins())) {
			current->Draw();
		}
	}

	count = (int) this->_plataformas.size();
	
	for(int i=0; i < count; i++) {
		Plataforma *plat = this->_plataformas[i];
		plat->Draw();
	}

	if(this->_player->getEstado() & TodasMonedasCogidas) {
		int posMap1, posMap2, posMap3, posMap4;

		posMap1 = this->_map->cols * 9 + 1;
		posMap2 = this->_map->cols * 8 + 1;
		posMap3 = this->_map->cols * 9 + 10;
		posMap4 = this->_map->cols * 8 + 10;
		if(this->_map->map[posMap1] == COLLISION_BLOCK && this->_map->map[posMap2] != COLLISION_BLOCK) {
			_g->BlitFrame(this->_textureFlechaIzq, 32, 288, 32, 32, false, false);
		}
		if(this->_map->map[posMap3] == COLLISION_BLOCK && this->_map->map[posMap4] != COLLISION_BLOCK) {
			_g->BlitFrame(this->_textureFlechaDer, 320, 288, 32, 32, false, false);
		}
	}

	if(this->_camisetaVisible) {
		this->drawCamiseta();
	}

	_player->Draw();

		// Si está el fantasma por ahí, oscurecemos la escena...
	if(this->_timeLeft <= 0) {
		_g->BlitColoredFrame(this->_textureSombra, 
							 0, 0, 
							 _g->WorldWidth, _g->WorldHeight, 
							 0.0f, 0.0f, 0.03125f, _valorOscuro, false, false, false);
	}

	count = (int) this->_enemigos.size();
	for(int i=0; i < count; i++) {
		Enemigo *enem = this->_enemigos[i];
		if(enem->getTipoEnemigo() == Fanty) {
			// enem->DrawShadow();
			enem->Draw();
		}
	}

	if(debugPaint) {
		this->_map->DebugPaint();
		this->_player->DebugPaint();
	}
}

void Room::drawSombraCamiseta() {
	_g->BlitShadow(this->_textureCamiseta, this->_camisetaX * 32, this->_camisetaY * 32, 32, 32, false, false);
}

void Room::drawCamiseta() {
	_g->BlitFrame(this->_textureCamiseta, this->_camisetaX * 32, this->_camisetaY * 32, 32, 32, false, false);
}

void Room::Dispose(void) {
	if(!this->_disposed) {
		int i;
		int count;

		count = (int) this->_plataformas.size();
		for(i = 0; i < count; i++) {
			delete this->_plataformas[i];
		}
		this->_plataformas.clear();

		count = (int) this->_monedas.size();
		for(i = 0; i < count; i++) {
			delete this->_monedas[i];
		}
		this->_monedas.clear();

		count = (int) this->_enemigos.size();
		for(i = 0; i < count; i++) {
			delete this->_enemigos[i];
		}
		this->_enemigos.clear();

		delete _back;
		this->_disposed = true;
	}
}

void Room::setTileFondo(TilesFondo tile) {
	_back->setTileFondo(tile);
}

Plataforma* Room::AddPlatform(TilePlataforma tipo, Direccion dir, char longitud, char tileX, char tileY, VECTOR2 tileSize) {
	Plataforma *plat = new Plataforma();
	plat->setTileSize(tileSize);
	plat->setTipoPlataforma(tipo);
	plat->setDireccion(dir);
	plat->setLongitud(longitud);
	plat->setPos(tileX, tileY);

	this->_drawables.push_back(plat);       
	this->_plataformas.push_back(plat);

	int mapPos = (tileY * MAP_COLS) + tileX;
	int incr = (dir == Horizontal)? 1 : MAP_COLS;
	for(int i=0; i<longitud; i++) {
		this->_map->map[mapPos] = COLLISION_BLOCK;
		mapPos += incr;
	}
	return plat;
}

Enemigo* Room::AddEnemy(TipoEnemigo tipo, Velocidad velocidad, char tileIni, char tileFin, char tileVert, VECTOR2 tileSize) {
	Enemigo *enem = FactoriaEnemigo::Create(tipo);

	enem->setTileSize(tileSize);
	enem->setPosition(tileIni, tileFin, tileVert);
	enem->setVelocidad(velocidad);

	this->_updatables.push_back(enem);
	this->_drawables.push_back(enem);
	this->_enemigos.push_back(enem);

	return enem;
}

void Room::AddFanty() {
	VECTOR2 vect;
	
	vect.x = 32;
	vect.y = 32;

	this->_fanty->setTileSize(vect);
	this->_fanty->setPosition(6, 6, 5);
	this->_fanty->setVelocidad(Lento);

	this->_updatables.push_back(this->_fanty);
	this->_enemigos.push_back(this->_fanty);

	this->_fanty->setAlpha(0.65f);
	this->_fanty->setPlayer(this->_player);
}

Coin* Room::AddCoin(char tileX, char tileY, VECTOR2 tileSize) {
	Coin *coin = new Coin();
	coin->setTileSize(tileSize);
	coin->setPos(tileX, tileY);

	this->_updatables.push_back(coin);
	this->_drawables.push_back(coin);

	this->_monedas.push_back(coin);

	return coin;
}

bool Room::loadRoom(ifstream &roomsFile) {
	unsigned char tileFondo;
	unsigned char moreData = 0x00;
	VECTOR2 vect;
	vect.x = 32;
	vect.y = 32;

	//Log::Out << "Loading room..." << endl;
	//Log::Out << "Creating Fanty..." << endl;

	this->_fanty = FactoriaEnemigo::Create(Fanty);

	// Leemos el tile de fondo.
	roomsFile.read(reinterpret_cast<char*>(&tileFondo), sizeof(unsigned char));
	this->_back = new Background();
	this->_back->setTileFondo((TilesFondo) tileFondo);

	//Log::Out << "   - Background: " << tileFondo << endl;

	// Leemos las plataformas:
	moreData = (unsigned char)roomsFile.peek();
	while(moreData != 0xFF) {
		unsigned char tipo;
		unsigned char dir;
		unsigned char lon;
		unsigned char tileX;
		unsigned char tileY;

		roomsFile.read(reinterpret_cast<char*>(&lon), sizeof(unsigned char));
		roomsFile.read(reinterpret_cast<char*>(&tipo), sizeof(unsigned char));
		roomsFile.read(reinterpret_cast<char*>(&dir), sizeof(unsigned char));
		roomsFile.read(reinterpret_cast<char*>(&tileX), sizeof(unsigned char));
		roomsFile.read(reinterpret_cast<char*>(&tileY), sizeof(unsigned char));
		this->AddPlatform((TilePlataforma)tipo, (Direccion)dir, lon, tileX, tileY, vect);

		moreData = (unsigned char)roomsFile.peek();
		// Log::Out << "Adding platform..." << endl;
	}
	// Saltamos el siguiente caracter...
	roomsFile.seekg(1, ios_base::cur);

	// Enemigos.
	moreData = (unsigned char)roomsFile.peek();
	while(moreData != 0xFF) {
		unsigned char tileVert;
		unsigned char tipoEnemigo;
		unsigned char velocidad;
		unsigned char tileIzq;
		unsigned char tileDer;

		roomsFile.read(reinterpret_cast<char*>(&tileVert), sizeof(unsigned char));
		roomsFile.read(reinterpret_cast<char*>(&tipoEnemigo), sizeof(unsigned char));
		roomsFile.read(reinterpret_cast<char*>(&velocidad), sizeof(unsigned char));
		roomsFile.read(reinterpret_cast<char*>(&tileIzq), sizeof(unsigned char));
		roomsFile.read(reinterpret_cast<char*>(&tileDer), sizeof(unsigned char));

		this->AddEnemy((TipoEnemigo) tipoEnemigo, (Velocidad) velocidad, tileIzq, tileDer, tileVert, vect);

		moreData = (unsigned char)roomsFile.peek();
		// Log::Out << "Adding Enemy: " << tipoEnemigo << endl;
	}

	// Saltamos el siguiente caracter...
	roomsFile.seekg(1, ios_base::cur);

	// Monedas.
	moreData = (unsigned char)roomsFile.peek();
	
	this->_initialTime = 0;

	while(moreData != 0xFF) {
		unsigned char tileX;
		unsigned char tileY;

		roomsFile.read(reinterpret_cast<char*>(&tileX), sizeof(unsigned char));
		roomsFile.read(reinterpret_cast<char*>(&tileY), sizeof(unsigned char));
		this->AddCoin(tileX, tileY, vect);
		
		this->_initialTime += 3000;

		moreData = (unsigned char)roomsFile.peek();
		// Log::Out << "Adding coin..." << endl;
	}

	this->_timeLeft = this->_initialTime;

	// Saltamos el siguiente caracter...
	roomsFile.seekg(1, ios_base::cur);

	moreData = (unsigned char)roomsFile.peek();

	return (moreData != 0xFF);
}
