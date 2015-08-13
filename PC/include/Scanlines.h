#include "IDrawable.h"

#include "GameData.h"
#include "GLFuncs.h"
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
	GLFuncs* gl;

    TEXTUREINFO* _texture;

	TEXTUREINFO* _texture2;
	TEXTUREINFO* _tv1;
	TEXTUREINFO* _tv2;
	TEXTUREINFO* _tv3;
	
	int _width;
	int _height;
	int _tilesX;
	int _tilesY;

	void InitScanlines(int tileWidth, 
			           int tileHeight, 
			           int tilesX, 
			           int tilesY);
};
