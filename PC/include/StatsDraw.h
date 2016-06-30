#pragma once

#include <string>
#include <sstream>
#include <iomanip>

#include "Graphics.h"
#include "AnimationPlayer.h"
#include "Player.h"

class StatsDraw
{
	Graphics *_g;

	AnimationPlayer _coinAnimPlayer;
	AnimationPlayer _uwolAnimPlayer;

public:
	StatsDraw();
	~StatsDraw();

	void DrawLives(int posX, int posY, int lives);
	void DrawCoins(int posX, int posY, int coins);
	void DrawLevel(int posX, int posY, int level);
	void DrawScore(int posX, int posY, int score);

	void Update(int milliSecs);
};

