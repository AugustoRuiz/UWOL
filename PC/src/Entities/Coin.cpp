#include "Coin.h"

Coin::Coin(void)
{
	this->Initialize();
}

Coin::~Coin(void)
{
	this->Dispose();
}

void Coin::Initialize()
{
	this->_animPlayer.setAnimation(Animation::Get("coin_flip"));
	this->_disposed = false;
	this->_g = Graphics::GetInstance();
	this->_rotationFactor = 1.0f;
}

void Coin::Dispose()
{
	if(!this->_disposed)
	{
		this->_disposed = true;
	}
}

void Coin::setTileSize(VECTOR2 tileSize)
{
	this->_tileSize = tileSize;
}

void Coin::setPos(char tileX, char tileY)
{
	this->_x = tileX;
	this->_y = tileY;
}

void Coin::DrawShadow()
{
	Frame* current = this->_animPlayer.GetCurrentFrame();
	_g->BlitShadow(current, 
		(this->_x * this->_tileSize.x), ((int)round(this->_y) * this->_tileSize.y),
		this->_tileSize.x, this->_tileSize.y, 
		false, false);
}

bool Coin::DrawWhenNoCoins()
{
	return true;
}

bool Coin::UpdateWhenNoCoins()
{
	return true;
}

void Coin::Draw()
{
	_g->BlitFrame(this->_animPlayer.GetCurrentFrame(), 
		          (int)(this->_x * this->_tileSize.x), 
		(int)(this->_y * this->_tileSize.y),
				  this->_tileSize.x,
				  this->_tileSize.y,
				  false, false);
}

void Coin::DrawInPos(int pX, int pY, float alpha)
{
	int sX = this->_tileSize.x;
	int sY = this->_tileSize.y;

	_g->BlitShadowAbs(this->_animPlayer.GetCurrentFrame(),
		pX + 4, pY + 4, 
		sX, sY, 
		false, false);

	_g->BlitColoredFrameAbs(this->_animPlayer.GetCurrentFrame(),
		          pX, pY, 
				  sX, sY,
				  alpha, alpha, alpha, 1.0, false, false, false);
}

void Coin::DebugPaint() {
	vector<VECTOR2> vertexes;
	RECTANGLEF cRect = this->getCollisionRect();
	int x1 = (int)(cRect.x);
	int x2 = (int)(cRect.x + cRect.width);
	int y1 = (int)(cRect.y);
	int y2 = (int)(cRect.y + cRect.height);

	vertexes.push_back(VECTOR2(x1, y1));
	vertexes.push_back(VECTOR2(x2, y1));
	vertexes.push_back(VECTOR2(x2, y1));
	vertexes.push_back(VECTOR2(x2, y2));
	vertexes.push_back(VECTOR2(x2, y2));
	vertexes.push_back(VECTOR2(x1, y2));
	vertexes.push_back(VECTOR2(x1, y2));
	vertexes.push_back(VECTOR2(x1, y1));

	_g->DrawPolyLines(vertexes, 1.0f, 1.0f, 1.0f, 1.0f);
}

RECTANGLEF Coin::getCollisionRect() {
	RECTANGLEF result;
	result.x = this->_colRect.x + (this->_x * this->_tileSize.x);
	result.y = this->_colRect.y + (this->_y * this->_tileSize.y);
	result.width = this->_colRect.width;
	result.height = this->_colRect.height;
	return result;
}

void Coin::setCollisionRect(const RECTANGLEF& rect) {
	this->_colRect = rect;
}

void Coin::Update(Uint32 milliSec, const Event& inputEvent)
{
	this->_animPlayer.Update((Uint32)(milliSec * this->_rotationFactor));
}

void Coin::SetFrame(int frame) {
	this->_animPlayer.setFrame(frame);
}
void Coin::SetTicks(int ticks) {
	this->_animPlayer.setTicks(ticks);
}
