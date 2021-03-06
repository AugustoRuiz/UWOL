#pragma once

#include "GameData.h"
#include "Stage.h"
#include "Presentacion.h"
#include "Portada.h"
#include "Piramide.h"
#include "EndGame.h"
#include "GameOver.h"
#include "Credits.h"
#include "Attract.h"
#include <string>
#include <iostream>
#include <iterator>
#include <algorithm>

#ifdef __APPLE__
#include "json.h"
#else
#include <json/json.h>
#endif

class Game
{
public:
	static Game* GetInstance();

	bool Initialize(int width, int height, bool fullscreen, const char* name);
	void Update(Uint32 mSecs);
	void Render();
	void Dispose();
	bool Running();
	void ShowCursor(bool show);

	void SetAttractMode(bool attract);
	void SetSaveAttract(bool save);
	void SetInertia(bool inertia);

	vector<Uint32> LoadAttractModeData();
	void SaveAttractModeData();

	void AddState(IGameState* state);
	void Restart();
protected:
	Game(void);
	~Game(void);

	static Game _instance;

private:
	Uint8 _attractTimes;

	bool _paused;
	bool _running;

	bool _savingStatus;
	bool _statusSaved;
	Uint32 _totalTicks;
	vector<Uint32> _eventBuffer;

	Graphics *_g;
	Program* _blitProgram;
	map<string, Program*> _blitProgramMap;
	InputManager *_input;

	Scanlines* _scanlines;
	bool _drawScanlines;

	map<string, IGameState*> _states;
	string _currentStatus;

	void loadResources();
	void handleInput(Event &currentEvent);
	//void updateAttractMode();
	void changeStatus(const string &oldStatus, const string &newStatus);

	void drawStatusMsg(const string& str);
	void SwapBuffers();

	Stage* _stage;
	Attract* _attract;
	MessageLine *_messageLine;
};
