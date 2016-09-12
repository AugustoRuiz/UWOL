#include "Stage.h"

Stage::Stage(void) {
	this->Name = "Stage";
	_g = Graphics::GetInstance();

	// Cargar los recursos...
	this->_frameSombra = new Frame("data/TileSombra.png");

	this->Player = new TPlayer();
	this->StatsDrawer = new StatsDraw();

	Room::StaticInit();

	this->RoomIndex = 0;
	this->CurrentRoom = this->loadRooms();

	this->_fading = false;
	this->_disposed = false;
}

Stage::~Stage(void) {
	Room::StaticDispose();
	this->Dispose();
}

void Stage::Restart() {
	this->RoomIndex = 0;
	this->CurrentRoom = this->Rooms[this->RoomIndex];
	for (Room* r : this->Rooms) {
		r->Completada = false;
		r->Restart();
	}
	this->Player->Initialize();
}

void Stage::OnEnter() {
	this->GoToRoom(this->RoomIndex);
}

void Stage::OnExit() {
	if (this->CurrentRoom != NULL) {
		this->CurrentRoom->OnExit();
	}
}

void Stage::Dispose() {
	if (!this->_disposed) {
		delete this->_frameSombra;
		this->disposeRooms();
		this->_disposed = true;
	}
}

void Stage::Draw() {
	this->CurrentRoom->Draw();

	// Cortamos por las bravas...
	_g->BlitFrameAbs(this->_frameSombra, 0, 0, _g->ScreenWidth, (int)_g->OffsetY - 1, false, false);
	_g->BlitFrameAbs(this->_frameSombra, 0, _g->ScreenHeight - (int)_g->OffsetY, _g->ScreenWidth, (int)_g->OffsetY, false, false);
	_g->BlitFrameAbs(this->_frameSombra, 0, 0, (int)_g->OffsetX, _g->ScreenHeight, false, false);
	_g->BlitFrameAbs(this->_frameSombra, _g->ScreenWidth - (int)_g->OffsetX, 0, (int)_g->OffsetX, _g->ScreenHeight, false, false);

	this->StatsDrawer->DrawLives(0, -32, this->Player->_vidas);
	this->DrawTime();
	this->StatsDrawer->DrawCoins(288, -32, this->Player->_coinsTaken);
	this->StatsDrawer->DrawLevel(0, 320, this->CurrentRoom->GetDepth());
	this->StatsDrawer->DrawScore(208, 320, this->Player->GetScore());

	if (this->_fading) {
		_g->BlitFrameAlphaAbs(this->_frameSombra, 0, 0, _g->ScreenWidth, _g->ScreenHeight, this->_fadeLevel, false, false);
	}
}

void Stage::DrawTime() {
	stringstream ss;
	ss << setfill('0') << setw(3) << (this->CurrentRoom->TimeLeft > 0 ? (this->CurrentRoom->TimeLeft / 1000) + 1 : 0);

	_g->DrawString(128, -16, "TIME", 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f);
	_g->DrawString(192, -16, "*", 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f);

	if (this->CurrentRoom->TimeLeft < 10000) {
		_g->DrawString(208, -16, ss.str(), 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f);
	}
	else {
		_g->DrawString(208, -16, ss.str(), 1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 1.0f);
	}
}

Program * Stage::GetProgram(void)
{
	return nullptr;
}

string Stage::Update(Uint32 milliSec, Event & inputEvent) {
	this->StatsDrawer->Update(milliSec);

	int estado = this->Player->getEstado();

	if (!this->_fading) {
		if (estado & SalidaIzq || estado & SalidaDer) {
			this->_fading = true;
			this->Player->AddScore(100);
			if (this->CurrentRoom->CheckTime) {
				this->Player->AddScore(((this->CurrentRoom->TimeLeft / 1000) + 1) * 5);
			}
			this->CurrentRoom->Completada = true;

			this->_fadeLevel = 0.0f;
			this->_fadeInc = 1.0f;
		}
		else {
			if (estado & Muerto) {
				if (this->Player->_vidas < 0) {
					return "GameOver";
				}
				else {
					return "Piramide";
				}
			}
			else {
				this->CurrentRoom->Update(milliSec, inputEvent);
			}
		}
	}
	else {
		this->_fadeLevel += this->_fadeInc * FADE_STEP * milliSec;

		if (this->_fadeLevel >= 1.0f) {
			// Está en negro, pasamos a la siguiente fase...
			this->_fadeLevel = 1.0f;
			this->_fadeInc = -1.0f;

			if (estado & SalidaDer) {
				this->RoomIndex += this->CurrentRoom->GetDepth() + 1;
			}

			if (estado & SalidaIzq) {
				this->RoomIndex += this->CurrentRoom->GetDepth();
			}

			if (this->RoomIndex >= (int)this->Rooms.size()) {
				// Comprobar si es el final del juego o no...
				this->RoomIndex = 0;
				// Fix guarro para la pirámide...
				this->CurrentRoom = this->Rooms[this->RoomIndex];

				if (this->Player->_coinsTaken > 255) {
					return "FinJuego_OK";
				}
				else {
					return "FinJuego_KO";
				}
			}
			else {
				// Fix guarro para la pirámide...
				this->CurrentRoom = this->Rooms[this->RoomIndex];
				return "Piramide";
			}
		}

		if (this->_fadeLevel <= 0.0f) {
			this->_fadeLevel = 0.0f;
			this->_fadeInc = 1.0f;
			this->_fading = false;
		}
	}

	return this->Name;
}

void Stage::GoToRoom(int roomIndex)
{
	Log::Out << "Current Room: " << this->RoomIndex << " of " << this->Rooms.size() << "." << endl;
	this->RoomIndex = roomIndex;
	this->CurrentRoom = this->Rooms[this->RoomIndex];
	this->CurrentRoom->setPlayer(this->Player);
	this->CurrentRoom->OnEnter();
	InputManager::GetInstance()->Reset();
}

Room* Stage::loadRooms()
{
	this->disposeRooms();

	Room *tmpRoom = NULL;

	istream *roomsFile = Pack::GetInstance()->GetStream("data/rooms.dat");
	VECTOR2 vect;

	vect.x = 32;
	vect.y = 32;

	int roomDepth = 1;
	int roomCount = 0;

	if (roomsFile->good())
	{
		bool more = true;
		while (more)
		{
			Room* room = new Room();
			more = room->loadRoom(roomsFile);
			room->SetDepth(roomDepth);

			if (++roomCount >= roomDepth) {
				++roomDepth;
				roomCount = 0;
			}

			this->Rooms.push_back(room);
		}
	}
	else
	{
		Log::Out << "Couldn't open file data/rooms.dat" << endl;
	}

	delete roomsFile;

	if (this->Rooms.size() == 0)
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
		((EFanty*)fanty)->setChaseable(tmpRoom->_player);

		this->Rooms.push_back(tmpRoom);
	}

	return this->Rooms[0];
}

void Stage::disposeRooms()
{
	size_t count = this->Rooms.size();
	for (size_t ii = 0; ii < count; ii++)
	{
		this->Rooms[ii]->Dispose();
	}
	this->Rooms.clear();
}
