#pragma once

#include "GameData.h"
#include "TextureMgr.h"
#include "Program.h"

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

	Program* GetProgram() const;
	void SetProgram(Program* p);

	void Init(const Json::Value& frame);
private:
	Program* _program;

};

