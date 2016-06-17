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

	this->_texture = Frame("data/Entrelazado.png");
	this->_texture2 = Frame("data/Entrelazado2.png");
	this->_tv1 = Frame("data/tvSim1.png");
	this->_tv2 = Frame("data/tvSim2.png");
	this->_tv3 = Frame("data/tvSim3.png");

	this->Mode = 1;

	g = Graphics::GetInstance();
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

		this->_tv1.Coords.ty2 = ((float)this->_height / this->_tv1.Texture->height);
		this->_tv2.Coords.ty2 = ((float)this->_height / this->_tv2.Texture->height);
		this->_tv3.Coords.ty2 = ((float)this->_height / this->_tv3.Texture->height);

		this->_texture2.Coords.ty2 = ((float) this->_width) / this->_texture2.Texture->width;

		for(int i=0; i < (this->_width / 32) + 1; i+=3)
		{
			g->BlitColoredFrameAbs(this->_tv1, i*32, 0, 32, this->_height, 1.0f, 1.0f, 1.0f, tvEffect, tvAdditive, false, false);
			g->BlitColoredFrameAbs(this->_tv2, (i+1)*32, 0, 32, this->_height, 1.0f, 1.0f, 1.0f, tvEffect, tvAdditive, false, false);
			g->BlitColoredFrameAbs(this->_tv3, (i+2)*32, 0, 32, this->_height, 1.0f, 1.0f, 1.0f, tvEffect, tvAdditive, false, false);
		}

		g->BlitFrameAlphaAbs(this->_texture2, 
					 0, 
					 0, 
					 this->_width, 
					 this->_height, 
					 0.2f, 
					 false, false);
	}
	else
	{
		this->_texture.Coords.tx2 = ((float) this->_width) / this->_texture.Texture->width;
		this->_texture.Coords.ty2 = ((float) this->_height) / this->_texture.Texture->height;

		g->BlitFrameAlphaAbs(this->_texture,
					 0, 
					 0, 
					 this->_width, 
					 this->_height, 
					 0.2f, false, false);
	}
}

bool Scanlines::DrawWhenNoCoins()
{
	return false;
}

void Scanlines::DrawShadow()
{
}
