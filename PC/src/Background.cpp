#include "Background.h"

Background::Background(void)
{
	_g = Graphics::GetInstance();
	InitBackground(Tile0, MAP_COLS, MAP_ROWS);
}

Background::~Background(void)
{

}

Background::Background(TilesFondo tileFondo,  
			   int tilesX, 
			   int tilesY)
{
	InitBackground(tileFondo, tilesX, tilesY);
}

void Background::InitBackground(TilesFondo tileFondo, 
			   int tilesX, 
			   int tilesY)
{

	this->_frames.push_back(Frame("data/TileFondo0.png", 0.0f, (float)tilesX, 0.0f, (float)tilesY));
	this->_frames.push_back(Frame("data/TileFondo1.png", 0.0f, (float)tilesX, 0.0f, (float)tilesY));
	this->_frames.push_back(Frame("data/TileFondo2.png", 0.0f, (float)tilesX, 0.0f, (float)tilesY));
	this->_frames.push_back(Frame("data/TileFondo3.png", 0.0f, (float)tilesX, 0.0f, (float)tilesY));

	this->setTileFondo(tileFondo);
	this->setSize(_g->WorldWidth, _g->WorldHeight);
	this->setTileCount(tilesX, tilesY);
}

void Background::setTileFondo(TilesFondo tileFondo)
{
	this->_tileFondo = tileFondo;

	switch(tileFondo)
	{
		case Tile0:
			_frame = this->_frames[0];
			break;
		case Tile1:
			_frame = this->_frames[1];
			break;
		case Tile2:
			_frame = this->_frames[2];
			break;
		case Tile3:
			_frame = this->_frames[3];
			break;
	}
}

void Background::setSize(int width, int height)
{
	this->_width = width;
	this->_height = height;
}

void Background::setTileCount(int tilesX, int tilesY)
{
	this->_tilesX = tilesX;
	this->_tilesY = tilesY;
}

void Background::Draw()
{
	_g->BlitFrame(this->_frame, 0, 0, this->_width, this->_height, false, false);
}
