#pragma once

#include "SDL.h"
#include "SDL_image.h"
#include "SDL_opengl.h"
#include "GameData.h"
#include <map>
#include <string>

using namespace std;

typedef map<string, TEXTUREINFO* > TEXTUREMAP;

class TextureMgr
{
public:
	TextureMgr(void);
	~TextureMgr(void);

	TEXTUREINFO* LoadTexture(const string &sFileName);
//	TEXTUREINFO* LoadTexture(SDL_RWops *rw);
	void DeleteTextures();

	static TextureMgr* GetInstance();

protected:
	static TextureMgr _instance;

private:
	TEXTUREMAP cache;
	TEXTUREINFO* GL_LoadTexture(SDL_Surface *textureSurf);
};
