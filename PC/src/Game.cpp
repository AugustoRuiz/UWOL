#include "Game.h"

Game *Game::GetInstance()
{
	return &_instance;
}

Game::Game(void)
{

}

Game::~Game(void)
{

}

bool Game::Running()
{
	return _running;
}

bool Game::Initialize(int width, int height, bool fullscreen, const char* name)
{
	Log::Out << "Game: Initializing..." << endl;

	_g = Graphics::GetInstance();
	_input = InputManager::GetInstance();

	Log::Out << "Game: Initializing screen (" << width << " x " << height << ")..." << endl;

	_running = _g->Initialize(width, height, 384, 320, fullscreen, name);

	_g->LightPosition.x = _g->WorldWidth / 2;
	_g->LightPosition.y = 0;

	if (_running)
	{
		Log::Out << "Game: Initialization OK." << endl;
	}
	else
	{
		Log::Out << "Game: Error initializing." << endl;
	}

	this->loadResources();

	Log::Out << "Game: Loading animations..." << endl;

	Animation::LoadAnimations("animations.json");

	Log::Out << "Game: Preparing Stage..." << endl;

	this->_stage = new Stage();

	this->AddState(new Portada());
	this->AddState(new Presentacion());
	this->AddState(new Credits());
	this->AddState(new Piramide((*(this->_stage))));
	this->AddState(this->_stage);
	this->AddState(new EndGame(true));
	this->AddState(new EndGame(false));
	this->AddState(new GameOver());

	_currentStatus = "Portada";

	Log::Out << "Game: Initializing Scanlines..." << endl;

	_scanlines = new Scanlines();
	_scanlines->setSize(width, height);

	this->_savingStatus = false;
	this->_statusSaved = false;
	this->_attractMode = false;

	this->_totalTicks = 0;

	return _running;
}

void Game::Dispose()
{
	_running = false;

	for (pair<string, IGameState*> pairState : _states) {
		pairState.second->Dispose();
		delete pairState.second;
	}

	delete _scanlines;

	TextureMgr::GetInstance()->DeleteTextures();
}

void Game::Update(Uint32 mSecs)
{
	Event currentEvent = InputManager::GetInstance()->Update(mSecs);
	this->handleInput(currentEvent);

	if (this->_attractMode || this->_savingStatus)
	{
		this->_totalTicks += mSecs;
	}

	if (this->_attractMode)
	{
		this->updateAttractMode();
	}

	if (_currentStatus == "")
	{
		throw "No next status!";
	}

	map<string, IGameState*>::iterator it = _states.find(_currentStatus);
	if (it != _states.end()) {
		IGameState *state = it->second;
		string oldStatus = _currentStatus;
		_currentStatus = state->Update(mSecs, currentEvent);
		// Si la ejecución del estado provoca un cambio de estado,
		// y ese estado tiene un nombre, lo inicializamos.
		if (oldStatus != _currentStatus) {
			state->OnExit();
			if (_states.find(_currentStatus) != _states.end()) {
				_states[_currentStatus]->OnEnter();
			}
		}
	}
	else {
		stringstream ss;
		ss << "Status " << _currentStatus << " not found!";
		throw ss.str();
	}
}

void Game::handleInput(Event &currentEvent) {
	if (currentEvent.Name == "KEY_DOWN") {
		ActionKeys key = (ActionKeys)(currentEvent.Data["key"].asInt());
		switch (key) {
		case ActionKeysExit:
			_running = false;
			break;
		case ActionKeysAliasing:
			aliasing = !aliasing;
			break;
		case ActionKeysScanlines:
			scanlines = !scanlines;
			break;
		case ActionKeysAltScanlines:
			this->_scanlines->Mode ^= 1;
			break;
		case ActionKeysDebug:
			debugPaint = !debugPaint;
			break;
		case ActionKeysStopRecording:
			// Dejamos de guardar después de guardar la pulsación de la tecla de grabar. Así la grabación 
			// durará hasta el momento en el que se ha pulsado la tecla.
			this->_savingStatus = false;
			break;
		case ActionKeysNextScreen:
			if (this->_currentStatus == "Stage") {
				map<string, IGameState*>::iterator it = _states.find(_currentStatus);
				if (it != _states.end()) {
					IGameState *state = it->second;
					Stage* s = (Stage*)state;
					int nextIdx = s->RoomIndex + 1;
					if (nextIdx < (int)s->Rooms.size()) {
						s->GoToRoom(nextIdx);
					}
				}
			}
			break;
		case ActionKeysPreviousScreen:
			if (this->_currentStatus == "Stage") {
				map<string, IGameState*>::iterator it = _states.find(_currentStatus);
				if (it != _states.end()) {
					IGameState *state = it->second;
					Stage* s = (Stage*)state;
					int nextIdx = s->RoomIndex - 1;
					if (nextIdx >= 0) {
						s->GoToRoom(nextIdx);
					}
				}
			}
			break;
		case ActionKeysAddCoins:
			if (this->_currentStatus == "Stage") {
				map<string, IGameState*>::iterator it = _states.find(_currentStatus);
				if (it != _states.end()) {
					IGameState *state = it->second;
					Stage* s = (Stage*)state;
					s->Player->_coinsTaken += 1;
				}
			}
			break;
		default:
			break;
		}

		if (this->_savingStatus) {
			this->_statusSaved = true;
			this->_eventBuffer.push_back(this->_totalTicks);
			this->_eventBuffer.push_back((Uint32)(0x3FFF & key));
		}
	}
	if (currentEvent.Name == "KEY_UP") {
		ActionKeys key = (ActionKeys)(currentEvent.Data["key"].asInt());
		if (this->_savingStatus) {
			this->_statusSaved = true;
			this->_eventBuffer.push_back(this->_totalTicks);
			this->_eventBuffer.push_back((Uint32)(0x4000 | (0x3FFF & key)));
		}
	}
}

void Game::updateAttractMode() {
	if (this->_evtBufferIterator != this->_eventBuffer.end()) {
		if (this->_totalTicks >= *this->_evtBufferIterator) {
			++this->_evtBufferIterator;
			Uint32 keyData = *this->_evtBufferIterator++;

			Event fakeEvent;

			fakeEvent.Data["key"] = (ActionKeys)(keyData & 0x3FFF);
			fakeEvent.Name = ((keyData & 0x4000) == 0x4000) ? "KEY_UP" : "KEY_DOWN";

			_input->SetKeyPressedState(fakeEvent);
		}
	}
	else {
		Log::Out << "Exiting attract mode..." << endl;
		this->_attractMode = false;
		_currentStatus = "Portada";
	}
}

void Game::Render()
{
	_g->Clear();

	map<string, IGameState*>::iterator it = _states.find(_currentStatus);
	if (it != _states.end())
	{
		IGameState *state = it->second;
		state->Draw();
	}

	if (scanlines)
	{
		this->_scanlines->Draw();
	}

	_g->SwapBuffers();
}

void Game::Restart() {
	this->_stage->Restart();
}

void Game::SetAttractMode(bool attract)
{
	this->_attractMode = attract;

	Log::Out << "SetAttractMode(" << (attract ? "true" : "false") << ")" << endl;

	if (attract)
	{
		_input->Enabled = false;
		this->_savingStatus = false;

		this->_totalTicks = 0;
		this->LoadAttractModeData();
		this->_evtBufferIterator = this->_eventBuffer.begin();
	}

	if (_currentStatus == "Stage")
	{
		map<string, IGameState*>::iterator it = _states.find(_currentStatus);
		if (it != _states.end())
		{
			((Stage*)it->second)->GoToRoom(0);
			this->_totalTicks = 0;
		}
	}
}

void Game::SetSaveAttract(bool save)
{
	Log::Out << "SetSaveAttract(" << (save ? "true" : "false") << ")" << endl;

	this->_savingStatus = save;

	if (save)
	{
		this->_attractMode = false;
		this->_totalTicks = 0;
	}
}

void Game::SaveAttractModeData()
{
	Log::Out << "Saving attract data to file status.dat." << endl;
	if (this->_statusSaved)
	{
		ofstream output("status.dat", ios::binary);
		copy(this->_eventBuffer.begin(), this->_eventBuffer.end(), ostreambuf_iterator<char>(output));
	}
}

void Game::LoadAttractModeData()
{
	ifstream input("status.dat", ios::binary);
	copy(istream_iterator<char>(input), istream_iterator<char>(), back_inserter(this->_eventBuffer));
}

void Game::ShowCursor(bool show)
{
	int showCursor = (show) ? SDL_ENABLE : SDL_DISABLE;
	SDL_ShowCursor(showCursor);
}

void Game::loadResources() {
	// Barra de progreso?
	_g->Clear();
	_g->SwapBuffers();

	Log::Out << "Game: Loading resources..." << endl;
	ifstream resourcesFile("resources.json", ios::binary);

	if (!resourcesFile.good()) {
		Log::Out << "Couldn't load file resources.json" << endl;
		return;
	}

	Json::Value root;
	resourcesFile >> root;
	Json::Value images = root["images"];
	if (Json::Value::null != images) {
		TextureMgr* texMgr = TextureMgr::GetInstance();
		for (Json::Value::iterator img = images.begin(); img != images.end(); ++img) {
			string file = img->asString();
			Log::Out << "Loading image " << file << endl;
			texMgr->LoadTexture(file);
		}
	}
	Json::Value music = root["music"];
	if (Json::Value::null != music) {
		MusicManager* musicMgr = MusicManager::GetInstance();
		for (Json::Value::iterator mus = music.begin(); mus != music.end(); ++mus) {
			string file = mus->asString();
			Log::Out << "Loading song " << file << endl;
			musicMgr->LoadMusic(mus->asString());
		}
	}
	Json::Value effects = root["fx"];
	if (Json::Value::null != effects) {
		MusicManager* musicMgr = MusicManager::GetInstance();
		for (Json::Value::iterator fx = effects.begin(); fx != effects.end(); ++fx) {
			string file = fx->asString();
			Log::Out << "Loading effect " << file << endl;
			musicMgr->LoadMusic(fx->asString());
		}
	}
	Log::Out << "Done!" << endl;
}

void Game::AddState(IGameState* state) {
	this->_states[state->Name] = state;
}