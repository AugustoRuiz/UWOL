#pragma once

#include "../Common.h"
#include "../Utils/Log.h"
#include <map>
#include <string>
#include <sstream>

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

	void dumpTextureInfo(GLuint texNumber);
	string getInternalFormatString(GLint value);
};
