#include "StatsDraw.h"

StatsDraw::StatsDraw()
{
	_g = Graphics::GetInstance();
	this->_uwolAnimPlayer.setAnimation(Animation::Get("uwol_walk_right"));
	this->_coinAnimPlayer.setAnimation(Animation::Get("coin_flip"));
}

StatsDraw::~StatsDraw()
{
}

void StatsDraw::DrawLives(int posX, int posY, int lives) {
	stringstream ss;
	ss << setfill('0') << setw(2) << lives;

	_g->BlitFrame(this->_uwolAnimPlayer.GetCurrentFrame(),
		posX, -32,
		32, 32,
		false, false);

	string livesStr = ss.str();
	_g->DrawString(posX + 48, posY + 16, livesStr, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 1.0f);

	_g->DrawString(posX + 32, posY + 16, "*", 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f);

}

void StatsDraw::DrawCoins(int posX, int posY, int coins) {
	stringstream ss;
	ss << setfill('0') << setw(3) << coins;

	string coinsStr = ss.str();
	_g->DrawString(posX + 48, posY + 16, coinsStr, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 1.0f);

	_g->BlitFrame(this->_coinAnimPlayer.GetCurrentFrame(), posX, posY, 32, 32, false, false);
	_g->DrawString(posX + 32, posY+16, "*", 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f);
}

void StatsDraw::DrawLevel(int posX, int posY, int level) {
	stringstream ss;
	ss << setfill('0') << setw(2) << level;
	string levelStr = ss.str();
	_g->DrawString(posX + 96, posY, levelStr, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 1.0f);

	_g->DrawString(posX, posY, "LEVEL", 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f);
	_g->DrawString(posX + 80, posY, "*", 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f);
}

void StatsDraw::DrawScore(int posX, int posY, int score) {
	stringstream ss;
	ss << setfill('0') << setw(5) << score;
	string scoreStr = ss.str();
	_g->DrawString(posX + 96, posY, scoreStr, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 1.0f);

	_g->DrawString(posX, posY, "SCORE", 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f);
	_g->DrawString(posX + 80, posY, "*", 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f);
}

void StatsDraw::Update(int milliSecs) {
	this->_coinAnimPlayer.Update(milliSecs);
	this->_uwolAnimPlayer.Update(milliSecs);
}
