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
	Frame current = this->_animPlayer.GetCurrentFrame();
	_g->BlitShadow(current, 
		(this->_x * this->_tileSize.x), (this->_y * this->_tileSize.y),
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
		          (this->_x * this->_tileSize.x), 
		          (this->_y * this->_tileSize.y), 
				  this->_tileSize.x,
				  this->_tileSize.y,
				  false, false);
}

void Coin::DrawInPos(int pX, int pY, float alpha)
{
	_g->BlitFrameAlphaAbs(this->_animPlayer.GetCurrentFrame(),
		          pX, 
		          pY, 
				  this->_tileSize.x,
				  this->_tileSize.y,
				  alpha, false, false);
}

void Coin::Update(Uint32 milliSec)
{
	this->_animPlayer.Update(milliSec);
}

void Coin::SetFrame(int frame) {
	this->_animPlayer.setFrame(frame);
}
void Coin::SetTicks(int ticks) {
	this->_animPlayer.setTicks(ticks);
}
