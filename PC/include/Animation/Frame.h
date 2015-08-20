#pragma once

#include "GameData.h"
#include "TextureMgr.h"
#ifdef __APPLE__
#include "json.h"
#else
#include <json/json.h>
#endif

class Frame
{
public:
	Frame();
	Frame(const string &path);
	Frame(const string &path, float tx1, float tx2, float ty1, float ty2);
	~Frame();

	TEXTUREINFO *Texture;
	TEXTURERECT Coords;

	void Init(Json::Value frame);
};

