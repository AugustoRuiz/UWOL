#include "IDrawable.h"

#include "GameData.h"
#include "Graphics.h"
#include "TextureMgr.h"

class Scanlines : public IDrawable
{
public:
	Scanlines(void);
	Scanlines(int tileWidth, 
			  int tileHeight, 
			  int tilesX, 
			  int tilesY);
	~Scanlines(void);
	void setTileCount(int tilesX, int tilesY);
	void setSize(int width, int height);
	void Draw();
	void DrawShadow(void);
	bool DrawWhenNoCoins();
	int Mode;

private:
	Graphics* g;

    Frame _texture;

	Frame _texture2;
	Frame _tv1;
	Frame _tv2;
	Frame _tv3;
	
	int _width;
	int _height;
	int _tilesX;
	int _tilesY;

	void InitScanlines(int tileWidth, 
			           int tileHeight, 
			           int tilesX, 
			           int tilesY);
};
