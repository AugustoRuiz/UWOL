#include "Scanlines.h"

Scanlines::Scanlines(void)
{
	InitScanlines(16, 16, 20, 20);
}

Scanlines::~Scanlines(void)
{
}

Scanlines::Scanlines(int tileWidth, 
			         int tileHeight, 
			         int tilesX, 
			         int tilesY)
{
	InitScanlines(tileWidth, tileHeight, tilesX, tilesY);
}

void Scanlines::InitScanlines(int tileWidth, 
			                  int tileHeight, 
			                  int tilesX, 
			                  int tilesY)
{
	this->setSize(tileWidth, tileHeight);
	this->setTileCount(tilesX, tilesY);

	this->_texture = TextureMgr::GetInstance()->LoadTexture("data/Entrelazado.png");

	this->_texture2 = TextureMgr::GetInstance()->LoadTexture("data/Entrelazado2.png");
	this->_tv1 = TextureMgr::GetInstance()->LoadTexture("data/tvSim1.png");
	this->_tv2 = TextureMgr::GetInstance()->LoadTexture("data/tvSim2.png");
	this->_tv3 = TextureMgr::GetInstance()->LoadTexture("data/tvSim3.png");

	this->Mode = 1;

	gl = GLFuncs::GetInstance();
}

void Scanlines::setSize(int width, int height)
{
	this->_width = width;
	this->_height = height;
}

void Scanlines::setTileCount(int tilesX, int tilesY)
{
	this->_tilesX = tilesX;
	this->_tilesY = tilesY;
}

void Scanlines::Draw()
{
	if(this->Mode == 0)
	{
		float tvEffect = 0.15f;
		bool tvAdditive = true;

		for(int i=0; i < (this->_width / 32) + 1; i+=3)
		{
			gl->BlitColoredRect(this->_tv1->texture, i*32, 0, 32, this->_height, 0, 0, 1.0f, (float) (this->_height / this->_tv1->height), 1.0f, 1.0f, 1.0f, tvEffect, tvAdditive);
			gl->BlitColoredRect(this->_tv2->texture, (i+1)*32, 0, 32, this->_height, 0, 0, 1.0f, (float) (this->_height / this->_tv2->height), 1.0f, 1.0f, 1.0f, tvEffect, tvAdditive);
			gl->BlitColoredRect(this->_tv3->texture, (i+2)*32, 0, 32, this->_height, 0, 0, 1.0f, (float) (this->_height / this->_tv3->height), 1.0f, 1.0f, 1.0f, tvEffect, tvAdditive);
		}

		gl->BlitRect(this->_texture2->texture, 
						0, 
						0, 
						this->_width, 
						this->_height, 
						0, 
						0, 
						(float) this->_width / this->_texture2->width, 
						1.0f,
						0.2f);
	}
	else
	{
		gl->BlitRect(this->_texture->texture, 
						0, 
						0, 
						this->_width, 
						this->_height, 
						0, 
						0, 
						(float) this->_width / this->_texture->width, 
						(float) this->_height / this->_texture->height,
						0.2f);
	}
}

bool Scanlines::DrawWhenNoCoins()
{
	return false;
}

void Scanlines::DrawShadow()
{
}
