#include "Stage.h"

Stage::Stage(void)
{
	this->Initialize();
}

Stage::~Stage(void)
{
	this->Dispose();
}

void Stage::Initialize()
{
	this->Name = "Stage";
	_g = Graphics::GetInstance();

	// Cargar los recursos...
	this->_uwolAnimPlayer.setAnimation(Animation::Get("uwol_walk_right"));
	this->_coinAnimPlayer.setAnimation(Animation::Get("coin_flip"));
	this->_frameSombra = Frame("data/TileSombra.png");

	this->_rotatingCoin = false;
	this->_movingUwol = false;

	this->_profundidad = 1;
	this->_currentRoomIdx = 0;

	this->_player = new Player();

	this->_currentRoom = this->loadRooms();
	this->_currentRoom->setPlayer(this->_player);
	this->_currentRoom->restartLevel();

	this->_fading = false;
	this->_disposed = false;
}

void Stage::Dispose()
{
	if(!this->_disposed)
	{
		this->disposeRooms();
		this->_disposed = true;
	}
}

void Stage::Draw()
{
	_currentRoom->Draw();

	// Cortamos por las bravas...
	_g->BlitFrameAbs(this->_frameSombra, 0, 0, _g->ScreenWidth, _g->OffsetY - 1, false, false);
	_g->BlitFrameAbs(this->_frameSombra, 0, _g->ScreenHeight - _g->OffsetY, _g->ScreenWidth, _g->OffsetY, false, false);
	_g->BlitFrameAbs(this->_frameSombra, 0, 0, _g->OffsetX, _g->ScreenHeight, false, false);
	_g->BlitFrameAbs(this->_frameSombra, _g->ScreenWidth - _g->OffsetX, 0, _g->OffsetX, _g->ScreenHeight, false, false);

	this->DrawLives();
	this->DrawTime();
	this->DrawCoins();
	this->DrawLevel();
	this->DrawScore();

	if(this->_fading)
	{
		_g->BlitFrameAlphaAbs(this->_frameSombra, 0, 0, _g->ScreenWidth, _g->ScreenHeight, this->_fadeLevel, false, false);
	}
}

void Stage::DrawLives()
{
	stringstream ss;
	ss << setfill('0') << setw(2) << this->_player->_vidas;

	_g->BlitFrame(this->_uwolAnimPlayer.GetCurrentFrame(), 
		0, - 32, 
		32, 32, 
		false, false);

	_g->DrawString(32, - 16,  "*",  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f);    
	_g->DrawString(48, - 16, ss.str(), 1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 1.0f);
}

void Stage::DrawTime()
{
	stringstream ss;
	ss << setfill('0') << setw(3) << (this->_currentRoom->_timeLeft > 0 ? (this->_currentRoom->_timeLeft / 1000) + 1 : 0);

	_g->DrawString(128, - 16, "TIME", 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f);
	_g->DrawString(192, - 16, "*", 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f);
	
	if(this->_currentRoom->_timeLeft < 10000)
	{
		_g->DrawString(208, - 16, ss.str(), 1.0f, 0.0f, 0.0f,  1.0f, 0.0f, 0.0f);
	}
	else
	{		
		_g->DrawString(208, - 16, ss.str(), 1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 1.0f);
	}
}

void Stage::DrawCoins()
{
	stringstream ss;
	ss << setfill('0') << setw(3) << this->_player->_coinsTaken;

	_g->BlitFrame(this->_coinAnimPlayer.GetCurrentFrame(), 288, - 32, 32, 32, false, false);
	_g->DrawString(320, - 16, "*", 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f);
	_g->DrawString(336, - 16, ss.str(), 1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 1.0f);
}

void Stage::DrawLevel()
{
	stringstream ss;
	ss << setfill('0') << setw(2) << this->_profundidad;
	_g->DrawString(96, 320, ss.str(), 1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 1.0f);
	_g->DrawString(0, 320, "LEVEL", 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f);
	_g->DrawString(80, 320, "*", 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f);
}

void Stage::DrawScore()
{
	stringstream ss;
	ss << setfill('0') << setw(5) << this->_player->_score;
	_g->DrawString(304, 320, ss.str(), 1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 1.0f);
	_g->DrawString(208, 320, "SCORE", 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f);
	_g->DrawString(288, 320, "*", 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f);
}

string Stage::Update(Uint32 milliSec, IGameState *lastState)
{
	this->_coinAnimPlayer.Update(milliSec);

	int estado;
	estado = this->_player->getEstado();
	if(estado != _ultimoEstadoUwol)
	{
		if(estado & Normal)
		{
			this->_uwolAnimPlayer.setAnimation(Animation::Get("uwol_walk_right"));
		}
		if(estado & Desnudo)
		{
			this->_uwolAnimPlayer.setAnimation(Animation::Get("uwol_walk_right_naked"));
		}
		if(estado & Muriendo)
		{
			this->_uwolAnimPlayer.setAnimation(Animation::Get("uwol_die"));
		}
		_ultimoEstadoUwol = estado;
	}
	this->_uwolAnimPlayer.Update(milliSec);

	if(!this->_fading)
	{
		if(estado & SalidaIzq || estado & SalidaDer)
		{
			this->_fading = true;
			this->_player->_score += 100;
			if(this->_currentRoom->_checkTime)
			{
				this->_player->_score += ( ( (this->_currentRoom->_timeLeft / 1000) + 1) * 5);
			}
			this->_fadeLevel = 0.0f;
			this->_fadeInc = 1.0f;
		}
		else
		{
			_currentRoom->Update(milliSec, lastState);
		}
	}
	else
	{
		MusicManager *musicMgr = MusicManager::GetInstance();
		musicMgr->SetMusicVol((int) (MIX_MAX_VOLUME - (MIX_MAX_VOLUME * this->_fadeLevel)));

		this->_fadeLevel += this->_fadeInc * FADE_STEP * milliSec;
		
		if(this->_fadeLevel >= 1.0f)
		{
			// Está en negro, pasamos a la siguiente fase...
			this->_fadeLevel = 1.0f;
			this->_fadeInc = -1.0f;
			
			musicMgr->StopMusic();

			if(estado & SalidaDer)
			{
				this->_currentRoomIdx += this->_profundidad + 1;
			}
			
			if(estado & SalidaIzq)
			{
				this->_currentRoomIdx += this->_profundidad;
			}
			
			this->_profundidad ++;
			if(this->_currentRoomIdx >= (int)this->rooms.size())
			{
				this->_currentRoomIdx = 0;
				this->_profundidad = 1;
			}
			this->_currentRoom = this->rooms[this->_currentRoomIdx];

			Log::Out << "Current Room: " << this->_currentRoomIdx << " of " << this->rooms.size() << "." << endl;
			
			this->_currentRoom->setPlayer(this->_player);
			this->_currentRoom->restartLevel();
		}

		if(this->_fadeLevel <= 0.0f)
		{
			this->_fadeLevel = 0.0f;
			this->_fadeInc = 1.0f;
			this->_fading = false;
		}
	}

	return this->Name;
}

void Stage::GoToRoom(int roomIndex, int depth)
{
	this->_fading = true;
	this->_fadeLevel = 0.0f;
	this->_fadeInc = 1.0f;
	this->_currentRoomIdx = roomIndex;
	this->_profundidad = depth;
}

Room* Stage::loadRooms()
{
	Room *tmpRoom = NULL;

	ifstream roomsFile("data/rooms.dat", ifstream::binary);
	VECTOR2 vect;
	
	vect.x = 32;
	vect.y = 32;

	if(roomsFile)
	{
		bool more = true;
		while(more)
		{	
			Room* room = new Room();
			more = room->loadRoom(roomsFile);
			this->rooms.push_back(room);
		}
		roomsFile.close();
	}
	else
	{
		Log::Out << "Couldn't open file data/rooms.dat" << endl;
	}

	if(this->rooms.size() == 0)
	{
		// Sacar los datos de una habitación.
		tmpRoom = new Room();
		// load platforms
		tmpRoom->AddPlatform(Plat0, Horizontal, 12, 0, 0, vect);
		tmpRoom->AddPlatform(Plat4, Horizontal, 12, 0, 9, vect);
		tmpRoom->AddPlatform(Plat5, Horizontal, 6, 3, 5, vect);
		tmpRoom->AddPlatform(Plat1, Horizontal, 1, 1, 7, vect);
		tmpRoom->AddPlatform(Plat2, Horizontal, 1, 9, 7, vect);

		// load coins.
		//
		tmpRoom->AddCoin(1, 2, vect);
		tmpRoom->AddCoin(5, 2, vect);
		tmpRoom->AddCoin(9, 2, vect);
		tmpRoom->AddCoin(2, 3, vect);
		tmpRoom->AddCoin(10, 3, vect);
		tmpRoom->AddCoin(3, 7, vect);
		tmpRoom->AddCoin(5, 7, vect);
		tmpRoom->AddCoin(7, 7, vect);

		// load enemies.
		tmpRoom->AddEnemy(Franky, Lento, 0, 11, 6, vect);
		tmpRoom->AddEnemy(Vampy, Rapido, 1, 10, 4, vect);
		tmpRoom->AddEnemy(Wolfy, Rapido, 0, 11, 1, vect);

		tmpRoom->setTileFondo(Tile3);

		Enemigo* fanty = tmpRoom->AddEnemy(Fanty, Lento, 3, 6, 4, vect);
		fanty->setAlpha(0.75f);
		fanty->setPlayer(tmpRoom->_player);

		rooms.push_back(tmpRoom);
	}

	return this->rooms[0];
}

void Stage::disposeRooms()
{
	size_t count = rooms.size();
    for(size_t ii=0; ii < count; ii++)
    {
       rooms[ii]->Dispose();
    }
	rooms.clear();
}
