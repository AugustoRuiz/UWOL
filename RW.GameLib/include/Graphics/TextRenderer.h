#pragma once

#include "../Common.h"
#include "TextureMgr.h"
#include "GLFuncs.h"
#include "Program.h"

class TextRenderer
{
public:
	TextRenderer();
	~TextRenderer();

	void DrawStringAlpha(int x, int y, int textSize, const string &text, float rTop, float gTop, float bTop, float rBot, float gBot, float bBot, float alpha);

private:
	Program* _program;
    TEXTUREINFO* _texture;
	void updateTexCoords(char currentChar, float* tx1, float* ty1, float* tx2, float* ty2);
};