#pragma once

#include "GameData.h"
#include "TextureMgr.h"
#include <json/json.h>

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

