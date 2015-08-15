#pragma once

#include "Frame.h"
#include "GLFuncs.h"
#include "TextRenderer.h"

class Graphics
{
	GLFuncs *_gl;
	TextRenderer _textRenderer;

	static Graphics _instance;

protected:
	Graphics();
	~Graphics();

public:
	static Graphics* GetInstance();

	int OffsetX;
	int OffsetY;
	int ScreenWidth;
	int ScreenHeight;
	int WorldWidth;
	int WorldHeight;

	VECTOR2 LightPosition;
	VECTOR2 MaxShadowOffset;

	SDL_Surface* Initialize(int screenWidth, int screenHeight, int worldWidth, int worldHeight, bool fullScreen);

	void BlitColoredFrameAbs(const Frame& frame, int x, int y, int width, int height, float red, float green, float blue, float alpha, bool additive, bool flipX, bool flipY);
	void BlitFrameAbs(const Frame& frame, int x, int y, int width, int height, bool flipX, bool flipY);
	void BlitFrameAlphaAbs(const Frame& frame, int x, int y, int width, int height, float alpha, bool flipX, bool flipY);
	void BlitShadowAbs(const Frame& frame, int x, int y, int width, int height, bool flipX, bool flipY);

	void BlitColoredFrame(const Frame& frame, int x, int y, int width, int height, float red, float green, float blue, float alpha, bool additive, bool flipX, bool flipY);
	void BlitFrame(const Frame& frame, int x, int y, int width, int height, bool flipX, bool flipY);
	void BlitFrameAlpha(const Frame& frame, int x, int y, int width, int height, float alpha, bool flipX, bool flipY);
	void BlitCenteredFrameAlpha(const Frame& frame, int width, int height, float alpha, bool filpX, bool flipY);
	void BlitShadow(const Frame& frame, int x, int y, int width, int height, bool flipX, bool flipY);

	void DrawString(int x, int y, const string &text, float rTop, float gTop, float bTop, float rBot, float gBot, float bBot);
	void DrawString(int x, int y, int textSize, const string &text, float rTop, float gTop, float bTop, float rBot, float gBot, float bBot);
	void DrawStringAbs(int x, int y, const string &text, float rTop, float gTop, float bTop, float rBot, float gBot, float bBot);
	void DrawStringAbs(int x, int y, int textSize, const string &text, float rTop, float gTop, float bTop, float rBot, float gBot, float bBot);

	void DrawStringAlpha(int x, int y, const string &text, float rTop, float gTop, float bTop, float rBot, float gBot, float bBot, float alpha);
	void DrawStringAlpha(int x, int y, int textSize, const string &text, float rTop, float gTop, float bTop, float rBot, float gBot, float bBot, float alpha);
	void DrawStringAlphaAbs(int x, int y, const string &text, float rTop, float gTop, float bTop, float rBot, float gBot, float bBot, float alpha);
	void DrawStringAlphaAbs(int x, int y, int textSize, const string &text, float rTop, float gTop, float bTop, float rBot, float gBot, float bBot, float alpha);

	void DrawPolyLines(const vector<VECTOR2> &vertexes, float red, float green, float blue, float alpha);
	void DrawPolyLinesAbs(const vector<VECTOR2> &vertexes, float red, float green, float blue, float alpha);
};

