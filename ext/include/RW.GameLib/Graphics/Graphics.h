#pragma once

#include "Program.h"
#include "../Animation/Frame.h"
#include "Shader.h"
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

	TEXTUREINFO* _framebufferTexture;

public:
	static Graphics* GetInstance();

	float OffsetX;
	float OffsetY;
	int ScreenWidth;
	int ScreenHeight;
	int WorldWidth;
	int WorldHeight;

	VECTOR2 LightPosition;
	VECTOR2 MaxShadowOffset;

	Program *DefaultProgram, *DefaultLineProgram;

	void ResetMVP();
	void OffsetMVP(float offsetX, float offsetY);

	bool Initialize(int screenWidth, int screenHeight, int worldWidth, int worldHeight, bool fullScreen, const char* name);

	void BlitColoredFrameAbs(Frame* frame, int x, int y, int width, int height, float red, float green, float blue, float alpha, bool additive, bool flipX, bool flipY);
	void BlitFrameAbs(Frame* frame, int x, int y, int width, int height, bool flipX, bool flipY);
	void BlitFrameAlphaAbs(Frame* frame, int x, int y, int width, int height, float alpha, bool flipX, bool flipY);
	void BlitShadowAbs(Frame* frame, int x, int y, int width, int height, bool flipX, bool flipY);

	void BlitColoredFrame(Frame* frame, int x, int y, int width, int height, float red, float green, float blue, float alpha, bool additive, bool flipX, bool flipY);
	void BlitFrame(Frame* frame, int x, int y, int width, int height, bool flipX, bool flipY);
	void BlitFrameAlpha(Frame* frame, int x, int y, int width, int height, float alpha, bool flipX, bool flipY);
	void BlitCenteredFrameAlpha(Frame* frame, int width, int height, float alpha, bool filpX, bool flipY);
	void BlitShadow(Frame* frame, int x, int y, int width, int height, bool flipX, bool flipY);

	void Clear();

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

	void SwapBuffers();
	GLuint CreateProgram(const vector<Shader*> shaders);

	TEXTUREINFO* GetFramebufferTexture();
};
