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
	_texMgr = TextureMgr::GetInstance();

	Log::Out << "Game: Initializing screen (" << width << " x " << height << ")..." << endl;

	_screen = _g->Initialize(width, height, 384, 320, fullscreen);

	_g->LightPosition.x = _g->WorldWidth / 2;
	_g->LightPosition.y = 0;

	_running = (NULL != _screen);

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

	this->AddState(new Portada());
	this->AddState(new Presentacion());
	this->AddState(new Stage());

	_currentStatus = "";
	_statesIt = _states.begin();

	Log::Out << "Game: Initializing Scanlines..." << endl;

	_scanlines = new Scanlines();
	_scanlines->setSize(width, height);

	this->_savingStatus = false;
	this->_statusSaved = false;
	this->_attractMode = false;

	this->_totalTicks = 0;

	return _running;
}

void Game::SetWindowName(char *name)
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

	_texMgr->DeleteTextures();
}

void Game::Update(Uint32 mSecs)
{
	SDL_Event event;
	SDL_PollEvent(&event);

	if (this->_attractMode || this->_savingStatus)
	{
		this->_totalTicks += mSecs;
	}

	if (this->_attractMode)
	{
		if (this->_evtBufferIterator != this->_eventBuffer.end())
		{
			if (this->_totalTicks >= *this->_evtBufferIterator)
			{
				++this->_evtBufferIterator;
				Uint32 keyData = *this->_evtBufferIterator++;

				SDL_KeyboardEvent fakeEvent;

				fakeEvent.keysym.sym = (SDLKey)(keyData & 0x3FFF);
				fakeEvent.type = ((keyData & 0x4000) == 0x4000) ? SDL_KEYUP : SDL_KEYDOWN;

				_input->SetKeyPressedState(&fakeEvent);

				//this->_running = (keyData & 0x3FFF) != SDLK_ESCAPE;
			}
		}
		else
		{
			Log::Out << "Exiting attract mode..." << endl;
			this->_attractMode = false;

			if (_states.find(_currentStatus) != _states.end()) {
				Stage* stage = (Stage*)_states[_currentStatus];
				stage->GoToRoom(0, 0);
			}
		}
	}

	switch (event.type) {
		// Look for a keypress 
	case SDL_KEYDOWN:
		if (!this->_attractMode)
		{
			_input->SetKeyPressedState(&(event.key));
		}

		if (event.key.keysym.sym == SDLK_ESCAPE)
		{
			_running = false;
		}
		if (event.key.keysym.sym == SDLK_SPACE)
		{
			aliasing = !aliasing;
		}
		if (event.key.keysym.sym == SDLK_s)
		{
			scanlines = !scanlines;
		}
		if (event.key.keysym.sym == SDLK_a)
		{
			this->_scanlines->Mode ^= 1;
		}

		if (event.key.keysym.sym == SDLK_d)
		{
			debugPaint = !debugPaint;
		}

		if (this->_savingStatus)
		{
			this->_statusSaved = true;
			this->_eventBuffer.push_back(this->_totalTicks);
			this->_eventBuffer.push_back((Uint32)(0x3FFF & event.key.keysym.sym));
		}

		// Dejamos de guardar después de guardar la pulsación de la tecla de grabar. Así la grabación 
		// durará hasta el momento en el que se ha pulsado la tecla.
		if (event.key.keysym.sym == SDLK_r)
		{
			this->_savingStatus = false;
		}
		break;
	case SDL_KEYUP:
		if (this->_savingStatus)
		{
			this->_statusSaved = true;

			this->_eventBuffer.push_back(this->_totalTicks);
			this->_eventBuffer.push_back((Uint32)(0x4000 | (0x3FFF & event.key.keysym.sym)));
		}
		if (!this->_attractMode)
		{
			_input->SetKeyPressedState(&(event.key));
		}
		break;
	case SDL_QUIT:
		//Log::WriteLog("Received SDL_QUIT... Ignoring...\n");
	   // _running = false;
		break;
	default:
		break;
	}

	if (_currentStatus != "")
	{
		map<string, IGameState*>::iterator it = _states.find(_currentStatus);
		if (it != _states.end()){
			IGameState *state = it->second;
			_currentStatus = state->Update(mSecs, state);
		}
	}
	else
	{
		if (this->_statesIt != _states.end()) {
			_currentStatus = this->_statesIt->first;
			++this->_statesIt;
		}
		else {
			_running = false;
		}
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

void Game::SetAttractMode(bool attract)
{
	this->_attractMode = attract;

	Log::Out << "SetAttractMode(" << (attract ? "true" : "false") << ")" << endl;

	if (attract)
	{
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
			((Stage*) it->second)->GoToRoom(0, 0);
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
			string &file = img->asString();
			Log::Out << "Loading image " << file << endl;
			texMgr->LoadTexture(file);
		}
	}
	Json::Value music = root["music"];
	if (Json::Value::null != music) {
		MusicManager* musicMgr = MusicManager::GetInstance();
		for (Json::Value::iterator mus = music.begin(); mus != music.end(); ++mus) {
			string &file = mus->asString();
			Log::Out << "Loading song " << file << endl;
			musicMgr->LoadMusic(mus->asString());
		}
	}
	Json::Value effects = root["fx"];
	if (Json::Value::null != effects) {
		MusicManager* musicMgr = MusicManager::GetInstance();
		for (Json::Value::iterator fx = effects.begin(); fx != effects.end(); ++fx) {
			string &file = fx->asString();
			Log::Out << "Loading effect " << file << endl;
			musicMgr->LoadMusic(fx->asString());
		}
	}
	Log::Out << "Done!" << endl;
}

void Game::AddState(IGameState* state) {
	this->_states[state->Name] = state;
}