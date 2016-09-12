#include "Background.h"

vector<Frame*> Background::_frames;
bool Background::_staticDisposed;

Background::Background(void)
{
	_g = Graphics::GetInstance();
	InitBackground(Tile0, MAP_COLS, MAP_ROWS);
}

Background::~Background(void) {

}

Background::Background(TilesFondo tileFondo,  
			   int tilesX, 
			   int tilesY)
{
	InitBackground(tileFondo, tilesX, tilesY);
}

void Background::StaticInit() {
	_frames.push_back(new Frame("data/TileFondo0.png"));
	_frames.push_back(new Frame("data/TileFondo1.png"));
	_frames.push_back(new Frame("data/TileFondo2.png"));
	_frames.push_back(new Frame("data/TileFondo3.png"));
}

void Background::StaticDispose() {
	if (!_staticDisposed) {
		_staticDisposed = true;
		for (Frame* f : _frames) {
			delete f;
		}
		_frames.clear();
	}
}

void Background::InitBackground(TilesFondo tileFondo, 
			   int tilesX, 
			   int tilesY)
{
	this->setTileFondo(tileFondo);
	this->setSize(_g->WorldWidth, _g->WorldHeight);
	this->setTileCount(tilesX, tilesY);
	this->updateDrawingCoords();
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
	updateDrawingCoords();
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

void Background::updateDrawingCoords() {
	this->_frame->Coords.tx1 = 0.0f;
	this->_frame->Coords.tx2 = (float)this->_tilesX;
	this->_frame->Coords.ty1 = 0.0f;
	this->_frame->Coords.ty2 = (float)this->_tilesY;
}