#include "Game.h"

#define ATTRACT_TIMES 3

Game *Game::GetInstance() {
	return &_instance;
}

Game::Game(void) {
	this->_blitProgramMap = map<string, Program*>();
}

Game::~Game(void) {

}

bool Game::Running() {
	return _running;
}

bool Game::Initialize(int width, int height, bool fullscreen, const char* name) {
	Log::Out << "Game: Initializing..." << endl;

	Pack::GetInstance()->Initialize("UWOL.pak");

	_g = Graphics::GetInstance();
	_input = InputManager::GetInstance();

	Log::Out << "Game: Initializing screen (" << width << " x " << height << ")..." << endl;

	_running = _g->Initialize(width, height, 384, 320, fullscreen, name);
	this->ShowCursor(false);

	_g->LightPosition.x = _g->WorldWidth / 2;
	_g->LightPosition.y = 0;

	if (_running) {
		Log::Out << "Game: Initialization OK." << endl;
	}
	else {
		Log::Out << "Game: Error initializing." << endl;
	}

	this->loadResources();

	Log::Out << "Game: Loading animations..." << endl;

	Animation::LoadAnimations("animations.json");

	Log::Out << "Game: Preparing Stage..." << endl;

	this->_stage = new Stage();

	this->_attract = new Attract();
	this->_attract->SetAttractData(this->LoadAttractModeData());
	this->_attract->SetRooms(this->_stage->Rooms);
	this->_attract->SetPlayer(this->_stage->Player);

	this->AddState(new Portada());
	this->AddState(new Presentacion());
	this->AddState(this->_attract);
	this->AddState(new Credits());
	this->AddState(new Piramide(this->_stage));
	this->AddState(this->_stage);
	this->AddState(new EndGame(true));
	this->AddState(new EndGame(false));
	this->AddState(new GameOver());

	_currentStatus = "Portada";

	this->_messageLine = new MessageLine();

	if (this->_blitProgram->ProgramId == 0) {
		_scanlines = new Scanlines();
		_scanlines->setSize(width, height);
		_drawScanlines = true;
	}
	else {
		_scanlines = NULL;
		_drawScanlines = false;
	}

	this->_savingStatus = false;
	this->_statusSaved = false;

	this->_totalTicks = 0;
	this->_attractTimes = 0;

	this->_paused = false;

	return _running;
}

void Game::Dispose() {
	_running = false;

	for (pair<string, IGameState*> pairState : _states) {
		pairState.second->Dispose();
		delete pairState.second;
	}

	if (this->_scanlines != NULL) {
		delete _scanlines;
	}

	TextureMgr::GetInstance()->DeleteTextures();
}

void Game::Update(Uint32 mSecs)
{
	string oldStatus = _currentStatus;

	this->_totalTicks += mSecs;

	Event currentEvent = InputManager::GetInstance()->Update(mSecs);

	this->_messageLine->Update(mSecs, currentEvent);

	this->handleInput(currentEvent);

	if (_currentStatus == "") {
		throw "No next status!";
	}

	if (!this->_paused) {
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
			}
			else {
				stringstream ss;
				ss << "No status found with name '" << _currentStatus << "'" << endl;
				Log::Out << ss.str() << endl;
				throw ss.str();
			}
		}
	}
}

void Game::changeStatus(const string &oldStatus, const string &newStatus) {
	string realNewStatus = newStatus;
	if (oldStatus != newStatus) {
		if (newStatus == "Presentacion") {
			this->Restart();
		}

		if (newStatus == "Attract") {
			this->_attractTimes = (this->_attractTimes + 1) % ATTRACT_TIMES;
			if (this->_attractTimes == 0) {
				realNewStatus = "Credits";
			}
		}

		map<string, IGameState*>::iterator it = _states.find(oldStatus);
		if (it != _states.end()) {
			IGameState *state = it->second;
			state->OnExit();
			if (_states.find(realNewStatus) != _states.end()) {
				_states[realNewStatus]->OnEnter();
				this->_currentStatus = realNewStatus;
			}
		}

		if (this->_currentStatus == "Stage" && this->_savingStatus) {
			Stage* stage = (Stage*)_states[this->_currentStatus];
			this->_totalTicks = 0;
			int currentRoom = stage->RoomIndex;
			this->_eventBuffer.push_back(this->_totalTicks);
			this->_eventBuffer.push_back(ROOM_START_EVENT + currentRoom);
			this->_eventBuffer.push_back(INERTIA_STATUS + (this->_stage->Player->hasInertia() ? 1 : 0));
			this->_eventBuffer.push_back(stage->CurrentRoom->GetRand());
		}
	}
}

void Game::handleInput(Event &currentEvent) {
	stringstream ss;
	vec3 color1 = vec3(0.9f);
	vec3 color2 = vec3(0.7f);
	if (currentEvent.Name == "KEY_DOWN") {
		ActionKeys key = (ActionKeys)(currentEvent.Data["key"].asInt());
		if (this->_savingStatus) {
			this->_statusSaved = true;
			this->_eventBuffer.push_back(this->_totalTicks);
			this->_eventBuffer.push_back((Uint32)(0x3FFF & key));
		}

		//if (key == ActionKeysAddCoins) {
		//	if (this->_currentStatus == "Stage") {
		//		map<string, IGameState*>::iterator it = _states.find(_currentStatus);
		//		if (it != _states.end()) {
		//			IGameState *state = it->second;
		//			Stage* s = (Stage*)state;
		//			s->Player->_coinsTaken += 1;
		//			vec3 color1 = vec3(0.9f);
		//			vec3 color2 = vec3(0.7f);
		//			this->_messageLine->ShowText("Added coin!", 1500, color1, color2);
		//		}
		//	}
		//}
	}
	if (currentEvent.Name == "KEY_UP") {
		ActionKeys key = (ActionKeys)(currentEvent.Data["key"].asInt());
		switch (key) {
		case ActionKeysExit:
			if (this->_currentStatus == "Presentacion") {
				_running = false;
			}
			else {
				this->_currentStatus = "Presentacion";
			}
			break;
			//case ActionKeysAliasing:
			//	aliasing = !aliasing;
			//	ss << "Antialiasing is " << (aliasing ? "ON" : "OFF");
			//	this->_messageLine->ShowText(ss.str(), 1500, vec3(0.9f), vec3(0.7f));
			//	break;
			//case ActionKeysScanlines:
			//	if (this->_scanlines == NULL && _blitPrograms.size() > 0) {
			//		if (_blitProgram == NULL) {
			//			_blitProgram = _blitPrograms[0];
			//		}
			//		else {
			//			vector<Program*>::iterator pos = std::find(_blitPrograms.begin(), _blitPrograms.end(), _blitProgram);
			//			if (pos == _blitPrograms.end()) {
			//				_blitProgram = _blitPrograms[0];
			//			}
			//			else {
			//				pos++;
			//				if (pos == _blitPrograms.end()) {
			//					pos = _blitPrograms.begin();
			//				}
			//			}
			//			_blitProgram = *pos;
			//		}
			//		ss << "Post-processing shader: " << ((_blitProgram != NULL) ? _blitProgram->ProgramId : 0);
			//		this->_messageLine->ShowText(ss.str(), 1500, vec3(0.9f), vec3(0.7f));
			//	}
			//	else {
			//		_drawScanlines = !_drawScanlines;
			//		ss << "Scanlines: " << (_drawScanlines ? "ON" : "OFF");
			//		this->_messageLine->ShowText(ss.str(), 1500, vec3(0.9f), vec3(0.7f));
			//	}
			//	break;
			//case ActionKeysAltScanlines:
			//	if (this->_scanlines != NULL) {
			//		this->_scanlines->Mode ^= 1;
			//		ss << "Scanline mode: " << this->_scanlines->Mode;
			//		this->_messageLine->ShowText(ss.str(), 1500, vec3(0.9f), vec3(0.7f));
			//	}
			//	break;
		//case ActionKeysDebug:
		//	debugPaint = !debugPaint;
		//	ss << "Debug paint: " << (debugPaint ? "ON" : "OFF");
		//	this->_messageLine->ShowText(ss.str(), 1500, vec3(0.9f), vec3(0.7f));
		//	break;
		case ActionKeysStopRecording:
			// Dejamos de guardar después de guardar la pulsación de la tecla de grabar. Así la grabación 
			// durará hasta el momento en el que se ha pulsado la tecla.
			if (this->_savingStatus) {
				this->_savingStatus = false;
				this->_messageLine->ShowText("Recording completed.", 1500, color1, color2);
			}
			break;
		case ActionKeysPause:
			this->_paused = !this->_paused;
			if (this->_paused) {				
				this->_messageLine->ShowText(("Pause! Press H to resume"), 1500, color1, color2);
			}
			else {
				this->_messageLine->ShowText((""), 1, color1, color2);
			}
			break;
		//case ActionKeysNextScreen:
		//	if (this->_currentStatus == "Stage") {
		//		map<string, IGameState*>::iterator it = _states.find(_currentStatus);
		//		if (it != _states.end()) {
		//			IGameState *state = it->second;
		//			Stage* s = (Stage*)state;
		//			int nextIdx = s->RoomIndex + 1;
		//			if (nextIdx < (int)s->Rooms.size()) {
		//				s->GoToRoom(nextIdx);
		//				this->_messageLine->ShowText("Next room", 1500, color1, color2);
		//			}
		//		}
		//	}
		//	break;
		//case ActionKeysPreviousScreen:
		//	if (this->_currentStatus == "Stage") {
		//		map<string, IGameState*>::iterator it = _states.find(_currentStatus);
		//		if (it != _states.end()) {
		//			IGameState *state = it->second;
		//			Stage* s = (Stage*)state;
		//			int nextIdx = s->RoomIndex - 1;
		//			if (nextIdx >= 0) {
		//				s->GoToRoom(nextIdx);
		//				this->_messageLine->ShowText("Previous room", 1500, color1, color2);
		//			}
		//		}
		//	}
		//	break;
		case ActionKeysToggleInertia:
			this->_stage->Player->toggleInertia();
			ss << "Inertia: " << (this->_stage->Player->hasInertia() ? "ON" : "OFF - Really??");
			this->_messageLine->ShowText(ss.str(), 1500, color1, color2);
			break;
		default:
			break;
		}

		if (this->_savingStatus) {
			this->_statusSaved = true;
			this->_eventBuffer.push_back(this->_totalTicks);
			this->_eventBuffer.push_back((Uint32)(0x4000 | (0x3FFF & key)));
		}
	}
}

void Game::Render() {
	Program* currentProgram = this->_blitProgram;

	_g->Clear();

	map<string, IGameState*>::iterator it = _states.find(_currentStatus);
	if (it != _states.end()) {
		IGameState *state = it->second;
		state->Draw();

		Program* p = state->GetProgram();
		if (p != nullptr) {
			this->_blitProgram = p;
		}
	}

	this->_messageLine->Draw();

	if (this->_drawScanlines && this->_scanlines != NULL) {
		this->_scanlines->Draw();
	}

	this->SwapBuffers();

	this->_blitProgram = currentProgram;
}

void Game::Restart() {
	this->_stage->Restart();
}

void Game::SetSaveAttract(bool save) {
	Log::Out << "SetSaveAttract(" << (save ? "true" : "false") << ")" << endl;
	this->_savingStatus = save;
}

void Game::SaveAttractModeData() {
	Log::Out << "Saving attract data to file status.dat." << endl;
	if (this->_statusSaved) {
		ofstream output("status.dat", ios::binary);
		for (Uint32 data : this->_eventBuffer) {
			output.write((char*)&data, sizeof(Uint32));;
		}
	}
}

vector<Uint32> Game::LoadAttractModeData() {
	vector<Uint32> result;
	ifstream input("status.dat", ios::binary);
	if (input.good()) {
		while (!input.eof()) {
			Uint32 data;
			input.read((char*)&data, sizeof(Uint32));
			result.push_back(data);
		}
	}
	return result;
}

void Game::SetInertia(bool inertia) {
	this->_stage->Player->setInertia(inertia);
}

void Game::ShowCursor(bool show) {
	int showCursor = (show) ? SDL_ENABLE : SDL_DISABLE;
	SDL_ShowCursor(showCursor);
}

void Game::drawStatusMsg(const string& str) {
	//Program* currentProgram = this->_blitProgram;
	//if (this->_blitProgramMap.size() > 0) {
	//	this->_blitProgram = this->_blitProgramMap.begin()->second;
	//}

	float grayValue = 0.4f;
	int fontSize = 8;

	Log::Out << str << endl;

	_g->Clear();
	_g->DrawString(0, _g->WorldHeight - 32, fontSize,
		str, grayValue, grayValue, grayValue, grayValue, grayValue, grayValue);

	this->SwapBuffers();

	//this->_blitProgram = currentProgram;
}

void Game::loadResources() {
	Log::Out << "Game: Loading resources..." << endl;
	istream* resourcesFile = Pack::GetInstance()->GetStream("resources.json");

	if (!resourcesFile->good()) {
		Log::Out << "Couldn't load file resources.json" << endl;
		return;
	}

	Json::Value root;
	(*resourcesFile) >> root;
	delete resourcesFile;

	Log::Out << "Game: Initializing Scanlines..." << endl;
	Json::Value shaders = root["outputShaders"];
	if (Json::Value::null != shaders) {
		ShaderMgr* shaderMgr = ShaderMgr::GetInstance();
		for (Json::Value::iterator itShaderDesc = shaders.begin(); itShaderDesc != shaders.end(); ++itShaderDesc) {
			vector<Shader*> shaderPtrs;
			Json::Value child = *itShaderDesc;
			string name = child["name"].asString();
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
			this->_blitProgramMap[name] = p;
			if (child.isMember("default") && child["default"].asBool()) {
				this->_blitProgram = p;
			}
		}
		if (this->_blitProgram == NULL && this->_blitProgramMap.size() > 0) {
			this->_blitProgram = (this->_blitProgramMap.end())->second;
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
	Program* p = this->_blitProgram;

	if (this->_paused) {
		p = this->_blitProgramMap["Sepia"];
	}

	if (p != NULL && p->ProgramId != 0) {
		p->Use();
		p->BindTextures();
		if (this->_paused) {
			p->SetUniform("sepiaRgb", vec3(1.2f, 1.0f, 0.8f));
		}
		p->SetUniform("iGlobalTime", (float)this->_totalTicks);
		p->SetUniform("MVP", GLFuncs::GetInstance()->StaticProjection);
		p->SetUniform("iResolution", vec2(this->_g->DisplayWidth, this->_g->DisplayHeight));
	}

	this->_g->SwapBuffers();
}