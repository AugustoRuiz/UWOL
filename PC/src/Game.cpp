#include "Game.h"

#define ROOM_START_EVENT 0x10000

Game *Game::GetInstance() {
	return &_instance;
}

Game::Game(void) {

}

Game::~Game(void) {

}

bool Game::Running() {
	return _running;
}

bool Game::Initialize(int width, int height, bool fullscreen, const char* name) {
	Log::Out << "Game: Initializing..." << endl;

	_g = Graphics::GetInstance();
	_input = InputManager::GetInstance();

	Log::Out << "Game: Initializing screen (" << width << " x " << height << ")..." << endl;

	_running = _g->Initialize(width, height, 384, 320, fullscreen, name);

	_g->LightPosition.x = _g->WorldWidth / 2;
	_g->LightPosition.y = 0;

	if (_running) {
		Log::Out << "Game: Initialization OK." << endl;
	} else {
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

	this->_messageLine = new MessageLine();

	if(this->_blitProgram->ProgramId == 0) {
		_scanlines = new Scanlines();
		_scanlines->setSize(width, height);
		_drawScanlines = true;
	} else {
		_scanlines = NULL;
		_drawScanlines = false;
	}

	this->_savingStatus = false;
	this->_statusSaved = false;
	this->_attractMode = false;

	this->_totalTicks = 0;

	return _running;
}

void Game::Dispose() {
	_running = false;

	for (pair<string, IGameState*> pairState : _states) {
		pairState.second->Dispose();
		delete pairState.second;
	}

	if(this->_scanlines != NULL) {
		delete _scanlines;
	}

	TextureMgr::GetInstance()->DeleteTextures();
}

void Game::Update(Uint32 mSecs)
{
	string oldStatus = _currentStatus;

	this->_messageLine->Update(mSecs);

	if (this->_attractMode) {
		this->updateAttractMode();
	}

	Event currentEvent = InputManager::GetInstance()->Update(mSecs);
	this->handleInput(currentEvent);

	this->_totalTicks += mSecs;

	if (_currentStatus == "") {
		throw "No next status!";
	}

	// Ha habido un cambio derivado de la gestión directa de la entrada de teclado?
	if (oldStatus != _currentStatus) {
		this->changeStatus(oldStatus, _currentStatus);
	}
	else {
		map<string, IGameState*>::iterator it = _states.find(_currentStatus);
		if (it != _states.end()) {
			IGameState *state = it->second;
			string oldStatus = _currentStatus;
			string newStatus = state->Update(mSecs, currentEvent);
			// Ha habido un cambio derivado de la actualización del estado actual?
			if (oldStatus != newStatus) {
				this->changeStatus(oldStatus, newStatus);
			}
		} else {
			stringstream ss;
			ss << "No status found with name '" << _currentStatus << "'" << endl;
			Log::Out << ss.str() << endl;
			throw ss.str();
		}
	}
}

void Game::changeStatus(const string &oldStatus, const string &newStatus) {
	if (oldStatus != newStatus) {
		map<string, IGameState*>::iterator it = _states.find(oldStatus);
		if (it != _states.end()) {
			IGameState *state = it->second;
			state->OnExit();
			if (_states.find(newStatus) != _states.end()) {
				_states[newStatus]->OnEnter();
				this->_currentStatus = newStatus;
			}
		}

		if (this->_currentStatus == "Stage" && this->_savingStatus) {
			Stage* stage = (Stage*)_states[this->_currentStatus];
			int currentRoom = stage->RoomIndex;
			this->_eventBuffer.push_back(this->_totalTicks);
			this->_eventBuffer.push_back(ROOM_START_EVENT + currentRoom);
		}
	}
}

void Game::handleInput(Event &currentEvent) {
	stringstream ss;
	if (currentEvent.Name == "KEY_DOWN") {
		ActionKeys key = (ActionKeys)(currentEvent.Data["key"].asInt());
		switch (key) {
		case ActionKeysExit:
			if (this->_attractMode) {
				this->SetAttractMode(false);
			} else {
				_running = false;
			}
			break;
		case ActionKeysAliasing:
			aliasing = !aliasing;
			ss << "Antialiasing is " << (aliasing ? "ON" : "OFF");
			this->_messageLine->ShowText(ss.str(), 1500, vec3(0.9f), vec3(0.7f));
			break;
		case ActionKeysScanlines:
			if(this->_scanlines == NULL && _blitPrograms.size()>0) {
				if (_blitProgram == NULL) {
					_blitProgram = _blitPrograms[0];
				} else {
					vector<Program*>::iterator pos = std::find(_blitPrograms.begin(), _blitPrograms.end(), _blitProgram);
					if (pos == _blitPrograms.end()) {
						_blitProgram = _blitPrograms[0];
					} else {
						pos++;
						if (pos == _blitPrograms.end()) {
							pos = _blitPrograms.begin();
						}
					}
					_blitProgram = *pos;
				}
				ss << "Post-processing shader: " << ((_blitProgram != NULL) ? _blitProgram->ProgramId : 0);
				this->_messageLine->ShowText(ss.str(), 1500, vec3(0.9f), vec3(0.7f));
			} else {
				_drawScanlines = !_drawScanlines;
				ss << "Scanlines: " << (_drawScanlines ? "ON" : "OFF");
				this->_messageLine->ShowText(ss.str(), 1500, vec3(0.9f), vec3(0.7f));
			}
			break;
		case ActionKeysAltScanlines:
			if(this->_scanlines != NULL) {
				this->_scanlines->Mode ^= 1;
				ss << "Scanline mode: " << this->_scanlines->Mode;
				this->_messageLine->ShowText(ss.str(), 1500, vec3(0.9f), vec3(0.7f));
			}
			break;
	case ActionKeysDebug:
			debugPaint = !debugPaint;
			ss << "Debug paint: " << (debugPaint ? "ON" : "OFF");
			this->_messageLine->ShowText(ss.str(), 1500, vec3(0.9f), vec3(0.7f));
			break;
		case ActionKeysStopRecording:
			// Dejamos de guardar después de guardar la pulsación de la tecla de grabar. Así la grabación 
			// durará hasta el momento en el que se ha pulsado la tecla.
			this->_savingStatus = false;
			this->_messageLine->ShowText("Recording completed.", 1500, vec3(0.9f), vec3(0.7f));
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
						this->_messageLine->ShowText("Next room", 1500, vec3(0.9f), vec3(0.7f));
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
						this->_messageLine->ShowText("Previous room", 1500, vec3(0.9f), vec3(0.7f));
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
					this->_messageLine->ShowText("Added coin!", 1500, vec3(0.9f), vec3(0.7f));
				}
			}
			break;
		case ActionKeysToggleInertia:
			this->_stage->Player->toggleInertia();
			ss << "Sissy mode: " << (this->_stage->Player->hasInertia() ? "OFF" : "ON");
			this->_messageLine->ShowText(ss.str(), 1500, vec3(0.9f), vec3(0.7f));
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
		if (this->_attractMode && (!currentEvent.Data["fake"].asBool())) {
			this->SetAttractMode(false);
		}
	}
}

void Game::updateAttractMode() {
	if (this->_evtBufferIterator != this->_eventBuffer.end()) {
		if (this->_totalTicks >= *this->_evtBufferIterator) {
			Uint32 keyData = *(this->_evtBufferIterator + 1);
			if ((keyData & ROOM_START_EVENT) == 0) {
				this->_evtBufferIterator += 2;

				Event fakeEvent;
				fakeEvent.Data["key"] = (ActionKeys)(keyData & 0x3FFF);
				fakeEvent.Data["fake"] = true;
				fakeEvent.Name = ((keyData & 0x4000) == 0x4000) ? "KEY_UP" : "KEY_DOWN";
				Log::Out << "Fake event: " << fakeEvent.Name << "(" << fakeEvent.Data["key"] << ")" << endl;
				_input->AddFakeEvent(fakeEvent);
			}
			else {
				Log::Out << "End of room - attract mode" << endl;
				this->SetAttractMode(false);
			}
		}
	} else {
		this->SetAttractMode(false);
	}
}

void Game::Render() {
	_g->Clear();

	map<string, IGameState*>::iterator it = _states.find(_currentStatus);
	if (it != _states.end()) {
		IGameState *state = it->second;
		state->Draw();
	}

	this->_messageLine->Draw();

	if (this->_drawScanlines && this->_scanlines != NULL) {
		this->_scanlines->Draw();
	}

	this->SwapBuffers();
}

void Game::Restart() {
	this->_stage->Restart();
}

void Game::SetAttractMode(bool attract) {
	this->_attractMode = attract;
	//_input->Enabled = !attract;

	Log::Out << "SetAttractMode(" << (attract ? "true" : "false") << ")" << endl;

	if (attract) {
		this->_savingStatus = false;
		this->_totalTicks = 0;
		this->LoadAttractModeData();
		this->_evtBufferIterator = this->_eventBuffer.begin();
	} else {
		this->_currentStatus = "Portada";
	}

	if (_currentStatus == "Stage") {
		map<string, IGameState*>::iterator it = _states.find(_currentStatus);
		if (it != _states.end()) {
			((Stage*)it->second)->GoToRoom(0);
			this->_totalTicks = 0;
		}
	}
}

void Game::SetSaveAttract(bool save) {
	Log::Out << "SetSaveAttract(" << (save ? "true" : "false") << ")" << endl;

	this->_savingStatus = save;

	if (save) {
		this->_attractMode = false;
		this->_totalTicks = 0;
	}
}

void Game::SaveAttractModeData() {
	Log::Out << "Saving attract data to file status.dat." << endl;
	if (this->_statusSaved) {
		ofstream output("status.dat", ios::binary);
		for (Uint32 data : this->_eventBuffer) {
			output.write((char*) &data, sizeof(Uint32));;
		}
	}
}

void Game::LoadAttractModeData() {
	ifstream input("status.dat", ios::binary);
	if (input.good()) {
		while (!input.eof()) {
			Uint32 data;
			input.read((char*)&data, sizeof(Uint32));
			this->_eventBuffer.push_back(data);
		}
	} else {
		this->SetAttractMode(false);
	}
}

void Game::SetInertia(bool inertia) {
	this->_stage->Player->setInertia(inertia);
}

void Game::ShowCursor(bool show) {
	int showCursor = (show) ? SDL_ENABLE : SDL_DISABLE;
	SDL_ShowCursor(showCursor);
}

void Game::drawStatusMsg(const string& str) {
	Program* currentProgram = this->_blitProgram;
	if (this->_blitPrograms.size() > 0) {
		this->_blitProgram = this->_blitPrograms[0];
	}

	float grayValue = 0.4f;
	int fontSize = 8;

	Log::Out << str << endl;

	_g->Clear();
	_g->DrawString(0, _g->WorldHeight - 32, fontSize, 
		str, grayValue, grayValue, grayValue, grayValue, grayValue, grayValue);

	this->SwapBuffers();

	this->_blitProgram = currentProgram;
}

void Game::loadResources() {
	Log::Out << "Game: Loading resources..." << endl;
	ifstream resourcesFile("resources.json", ios::binary);

	if (!resourcesFile.good()) {
		Log::Out << "Couldn't load file resources.json" << endl;
		return;
	}

	Json::Value root;
	resourcesFile >> root;

	Log::Out << "Game: Initializing Scanlines..." << endl;
	Json::Value shaders = root["outputShaders"];
	if (Json::Value::null != shaders) {
		ShaderMgr* shaderMgr = ShaderMgr::GetInstance();
		for (Json::Value::iterator itShaderDesc = shaders.begin(); itShaderDesc != shaders.end(); ++itShaderDesc) {
			vector<Shader*> shaderPtrs;
			Json::Value child = *itShaderDesc;
			Json::Value pathArray = child["shaderPaths"];
			for (Json::Value::iterator itShaderPath = pathArray.begin(); itShaderPath != pathArray.end(); ++itShaderPath) {
				string shaderFile = itShaderPath->asString();
				ShaderType shaderType = shaderFile.find(".vertex") == string::npos ? Fragment : Vertex;
				stringstream ss;
				ss << "Loading " << ((shaderType == Vertex) ? "vertex" : "fragment") << " shader: " << shaderFile;
				drawStatusMsg(ss.str());
				shaderPtrs.push_back(shaderMgr->LoadShader(shaderFile, shaderType));				
			}
			Program* p = new Program(shaderPtrs);
			if (p->ProgramId != 0) {
				p->Textures.push_back(_g->GetFramebufferTexture());
			}
			Json::Value texArray = child["additionalTextures"];
			for (Json::Value::iterator itTexturePath = texArray.begin(); itTexturePath != texArray.end(); ++itTexturePath) {
				string texPath = itTexturePath->asString();
				p->Textures.push_back(Frame(texPath).Texture);
			}
			this->_blitPrograms.push_back(p);
			if (child.isMember("default") && child["default"].asBool()) {
				this->_blitProgram = p;
			}
		}
		if (this->_blitProgram == NULL && this->_blitPrograms.size() > 0) {
			this->_blitProgram = this->_blitPrograms.back();
		}
	}

	Json::Value images = root["images"];
	if (Json::Value::null != images) {
		TextureMgr* texMgr = TextureMgr::GetInstance();
		for (Json::Value::iterator img = images.begin(); img != images.end(); ++img) {
			string file = img->asString();
			stringstream ss;
			ss << "Loading image: " << file;
			drawStatusMsg(ss.str());
			texMgr->LoadTexture(file);
		}
	}
	Json::Value music = root["music"];
	if (Json::Value::null != music) {
		MusicManager* musicMgr = MusicManager::GetInstance();
		for (Json::Value::iterator mus = music.begin(); mus != music.end(); ++mus) {
			string file = mus->asString();
			stringstream ss;
			ss << "Loading song " << file;
			drawStatusMsg(ss.str());
			musicMgr->LoadMusic(mus->asString());
		}
	}
	Json::Value effects = root["fx"];
	if (Json::Value::null != effects) {
		MusicManager* musicMgr = MusicManager::GetInstance();
		for (Json::Value::iterator fx = effects.begin(); fx != effects.end(); ++fx) {
			stringstream ss;
			string file = fx->asString();
			ss << "Loading effect " << file;
			drawStatusMsg(ss.str());
			musicMgr->LoadMusic(fx->asString());
		}
	}
	drawStatusMsg("Done!");
}

void Game::AddState(IGameState* state) {
	this->_states[state->Name] = state;
}

void Game::SwapBuffers() {
	if (this->_blitProgram != NULL && this->_blitProgram->ProgramId != 0) {
		this->_blitProgram->Use();
		this->_blitProgram->BindTextures();
		this->_blitProgram->SetUniform("iGlobalTime", (float)this->_totalTicks);
		this->_blitProgram->SetUniform("MVP", GLFuncs::GetInstance()->MVP);
		this->_blitProgram->SetUniform("iResolution", vec2(this->_g->ScreenWidth, this->_g->ScreenHeight));
	}

	this->_g->SwapBuffers();
}