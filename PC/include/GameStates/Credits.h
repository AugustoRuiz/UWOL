#pragma once

#include <fstream>
#include "GameData.h"
#include "Stage.h"

class Credits : public IGameState, public IChaseable
{
public:
	Credits(void);
	~Credits(void);

	// Implementación de IGameState.
	virtual void OnEnter(void) override;
	virtual void OnExit(void) override;
	virtual string Update(Uint32 milliSec, Event & inputEvent) override;
	virtual void Draw(void) override;
	virtual void Dispose(void) override;
	virtual Program* GetProgram(void) override;

private:
	void drawCentered(int y, const string &str);
	void drawFrame(int *y, Frame* frame);

	bool _disposed; //, _goNext;
	Graphics *_g;
	Sound *_tune;

	EFanty *_fanty;

	//float _yOffset;
	float _rTitleTop, _gTitleTop, _bTitleTop, _rTitleBot, _gTitleBot, _bTitleBot;
	float _rTextTop, _gTextTop, _bTextTop, _rTextBot, _gTextBot, _bTextBot; //, _textAlpha;
	float _rGoldTop, _gGoldTop, _bGoldTop, _rGoldBot, _gGoldBot, _bGoldBot;

	vector<float> _textPauseY;

	Uint32 _pauseTicks, _currentPauseIdx;
	int _textSize;
	int _maxPosY;

	Frame *_uwolIcon;
	Frame *_shadow;

	AnimationPlayer *_uwolAnimPlayer;
	float _uwolX;
	float _uwolVX;

	// Inherited via IChaseable
	virtual bool ShouldChase() override;
	virtual VECTOR2F GetPosition() override;
};
