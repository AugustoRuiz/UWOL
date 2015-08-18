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

bool Game::Initialize(int width, int height, bool fullscreen)
{
	Log::Out << "Game: Initializing..." << endl;

	_g = Graphics::GetInstance();
	_input = InputManager::GetInstance();

	Log::Out << "Game: Initializing screen (" << width << " x " << height << ")..." << endl;

	_running = _g->Initialize(width, height, 384, 320, fullscreen);

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

void Game::SetWindowName(const char *name)
{
	SDL_WM_SetCaption(name, NULL);
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
		this->_statesIt = it;
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
		SDLKey keySym = (SDLKey)(currentEvent.Data["key"].asInt());
		switch (keySym) {
			case SDLK_ESCAPE:
				_running = false;
				break;
			case SDLK_SPACE:
				aliasing = !aliasing;
				break;
			case SDLK_s:
				scanlines = !scanlines;
					break;
			case SDLK_a:
				this->_scanlines->Mode ^= 1;
				break;
			case SDLK_d:
				debugPaint = !debugPaint;
				break;
			case SDLK_r:
				// Dejamos de guardar después de guardar la pulsación de la tecla de grabar. Así la grabación 
				// durará hasta el momento en el que se ha pulsado la tecla.
				this->_savingStatus = false;
				break;
			default:
				break;
		}

		if (this->_savingStatus) {
			this->_statusSaved = true;
			this->_eventBuffer.push_back(this->_totalTicks);
			this->_eventBuffer.push_back((Uint32)(0x3FFF & keySym));
		}
	}
	if (currentEvent.Name == "KEY_UP") {
		SDLKey keySym = (SDLKey)(currentEvent.Data["key"].asInt());
		if (this->_savingStatus) {
			this->_statusSaved = true;
			this->_eventBuffer.push_back(this->_totalTicks);
			this->_eventBuffer.push_back((Uint32)(0x4000 | (0x3FFF & keySym)));
		}
	}
	}

void Game::updateAttractMode() {
	if (this->_evtBufferIterator != this->_eventBuffer.end()) {
		if (this->_totalTicks >= *this->_evtBufferIterator) {
				++this->_evtBufferIterator;
				Uint32 keyData = *this->_evtBufferIterator++;

				SDL_KeyboardEvent fakeEvent;

				fakeEvent.keysym.sym = (SDLKey)(keyData & 0x3FFF);
				fakeEvent.type = ((keyData & 0x4000) == 0x4000) ? SDL_KEYUP : SDL_KEYDOWN;

				_input->SetKeyPressedState(&fakeEvent);

				//this->_running = (keyData & 0x3FFF) != SDLK_ESCAPE;
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
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

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

	SDL_GL_SwapBuffers();
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
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	SDL_GL_SwapBuffers();

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