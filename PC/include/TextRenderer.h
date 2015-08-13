#include "TextureMgr.h"
#include "GLFuncs.h"
#include "GameData.h"

#pragma once

class TextRenderer
{
public:
	TextRenderer();
	~TextRenderer();

	void DrawString(int x, int y, const string &text, float rTop, float gTop, float bTop, float rBot, float gBot, float bBot);

private:
    TEXTUREINFO *texture;

	void updateTexCoords(char currentChar, float *tx1, float *tx2, float *ty1, float *ty2);
};