#include "Graphics.h"

Graphics *Graphics::GetInstance()
{
	return &_instance;
}

Graphics::Graphics()
{
	this->_gl = GLFuncs::GetInstance();

	this->MaxShadowOffset.x = 16;
	this->MaxShadowOffset.y = 20;
}

Graphics::~Graphics()
{
}

TEXTUREINFO* Graphics::GetFramebufferTexture() 
{
	return this->_framebufferTexture;
}

void Graphics::ResetMVP()
{
	this->_gl->ResetMVP();
}

void Graphics::OffsetMVP(float offsetX, float offsetY)
{
	this->_gl->OffsetMVP(offsetX, offsetY);
}

bool Graphics::Initialize(int screenWidth, int screenHeight, int worldWidth, int worldHeight, bool fullscreen, const char* name) {
	bool result = false;

	this->ScreenWidth = screenWidth;
	this->ScreenHeight = screenHeight;
	this->WorldWidth = worldWidth;
	this->WorldHeight = worldHeight;
	this->OffsetX = (this->ScreenWidth - this->WorldWidth) / 2.0f;
	this->OffsetY = (this->ScreenHeight - this->WorldHeight) / 2.0f;
	result = (NULL != _gl->Initialize(this->ScreenWidth, this->ScreenHeight, fullscreen, name));

	this->_framebufferTexture = new TEXTUREINFO();
	this->_framebufferTexture->texture = _gl->GetFramebufferTexture();
	this->_framebufferTexture->width = this->ScreenWidth;
	this->_framebufferTexture->height = this->ScreenHeight;

	vector<string> vertexShaders = { "data/shaders/Default.150.vertex" };
	vector<string> fragmentShaders = { "data/shaders/TexturedColored.150.fragment" };

	vector<string> lineVertexShaders = { "data/shaders/Line.150.vertex" };
	vector<string> lineFragmentShaders = { "data/shaders/Color.150.fragment" };

	this->DefaultProgram = new Program(vertexShaders, fragmentShaders);
	this->DefaultLineProgram = new Program(lineVertexShaders, lineFragmentShaders);

	return result;
}

void Graphics::BlitColoredFrameAbs(const Frame& frame, int x, int y, int width, int height,
	float red, float green, float blue, float alpha, bool additive, bool flipX, bool flipY) {

	Program* program = frame.GetProgram();
	if (program == NULL) program = this->DefaultProgram;

    if(program->ProgramId != 0) {
		if (program->Textures.size() > 0) {
			program->Textures[0] = frame.Texture;
		} else {
			program->Textures.push_back(frame.Texture);
		}

		program->Use();
		program->BindTextures();
		program->SetUniform("MVP", _gl->MVP);
    } else {
    	glBindTexture(GL_TEXTURE_2D, frame.Texture->texture);
    }

	_gl->BlitColoredRect(x, y, width, height,
		flipX ? frame.Coords.tx2 : frame.Coords.tx1, flipY ? frame.Coords.ty2 : frame.Coords.ty1,
		flipX ? frame.Coords.tx1 : frame.Coords.tx2, flipY ? frame.Coords.ty1 : frame.Coords.ty2,
		red, green, blue, alpha, additive);
}

void Graphics::BlitFrameAbs(const Frame& frame, int x, int y, int width, int height, bool flipX, bool flipY)
{
	this->BlitColoredFrameAbs(frame, x, y, width, height, 1.0f, 1.0f, 1.0f, 1.0f, false, flipX, flipY);
}

void Graphics::BlitFrameAlphaAbs(const Frame& frame, int x, int y, int width, int height, float alpha, bool flipX, bool flipY)
{
	this->BlitColoredFrameAbs(frame, 
		x, y, 
		width, height, 
		1.0f, 1.0f, 1.0f, alpha, 
		false, 
		flipX, flipY);
}

void Graphics::BlitShadowAbs(const Frame& frame, int x, int y, int width, int height, bool flipX, bool flipY)
{
	this->BlitColoredFrameAbs(frame, 
		x, y, 
		width, height, 
		0.0f, 0.0f, 0.0f, 0.5f, 
		false, 
		flipX, flipY);
}

void Graphics::DrawPolyLinesAbs(const vector<VECTOR2> &vertexes, float red, float green, float blue, float alpha) {
	if(this->DefaultLineProgram->ProgramId != 0) {
		this->DefaultLineProgram->Use();
		this->DefaultLineProgram->SetUniform("vertexColor", glm::vec4(red, green, blue, alpha));
		this->DefaultLineProgram->SetUniform("MVP", _gl->MVP);
	}

	_gl->DrawPolyLine(vertexes, red, green, blue, alpha);
}

void Graphics::SwapBuffers()
{
	_gl->SwapBuffers();
}

void Graphics::DrawStringAbs(int x, int y, const string &text, float rTop, float gTop, float bTop, float rBot, float gBot, float bBot) {
	this->_textRenderer.DrawStringAlpha(x, y, 
		16, 
		text, 
		rTop, gTop, bTop, 
		rBot, gBot, bBot, 
		1.0f);
}

void Graphics::DrawStringAbs(int x, int y, int textSize, const string &text, float rTop, float gTop, float bTop, float rBot, float gBot, float bBot) {
	this->_textRenderer.DrawStringAlpha(x, y, 
		textSize, 
		text, 
		rTop, gTop, bTop, 
		rBot, gBot, bBot, 
		1.0f);
}

void Graphics::DrawStringAlphaAbs(int x, int y, const string &text, float rTop, float gTop, float bTop, float rBot, float gBot, float bBot, float alpha) {
	this->_textRenderer.DrawStringAlpha(x, y, 
		16, 
		text, 
		rTop, gTop, bTop, 
		rBot, gBot, bBot, 
		alpha);
}

void Graphics::DrawStringAlphaAbs(int x, int y, int textSize, const string &text, float rTop, float gTop, float bTop, float rBot, float gBot, float bBot, float alpha) {
	this->_textRenderer.DrawStringAlpha(x, y, 
		textSize, 
		text, 
		rTop, gTop, bTop, 
		rBot, gBot, bBot, 
		alpha);
}

void Graphics::BlitColoredFrame(const Frame& frame, int x, int y, int width, int height,
	float red, float green, float blue, float alpha, bool additive, bool flipX, bool flipY) {
	this->BlitColoredFrameAbs(frame, 
		(int)(x + this->OffsetX), (int)(y + this->OffsetY), 
		width, height, 
		red, green, blue, alpha, 
		additive, 
		flipX, flipY);
}

void Graphics::BlitFrame(const Frame& frame, int x, int y, int width, int height, bool flipX, bool flipY)
{
	this->BlitColoredFrameAbs(frame, 
		(int)(x + this->OffsetX), (int)(y + this->OffsetY),
		width, height,
		1.0f, 1.0f, 1.0f, 1.0f, 
		false, 
		flipX, flipY);
}

void Graphics::BlitFrameAlpha(const Frame& frame, int x, int y, int width, int height, float alpha, bool flipX, bool flipY)
{
	this->BlitColoredFrameAbs(frame, 
		(int)(x + this->OffsetX), (int)(y + this->OffsetY),
		width, height,
		1.0f, 1.0f, 1.0f, alpha, 
		false, 
		flipX, flipY);
}

void Graphics::BlitCenteredFrameAlpha(const Frame& frame, int width, int height, float alpha, bool flipX, bool flipY)
{
	int x = (this->WorldWidth - width) / 2;
	int y = (this->WorldHeight - height) / 2;

	this->BlitColoredFrameAbs(frame, 
		(int)(x + this->OffsetX), (int)(y + this->OffsetY),
		width, height,
		1.0f, 1.0f, 1.0f, alpha, 
		false, 
		flipX, flipY);
}

void Graphics::BlitShadow(const Frame& frame, int x, int y, int width, int height, bool flipX, bool flipY)
{
	int x1, x2, y1, y2;
	int finalWidth, finalHeight;
	float halfW, halfH;

	halfW = (float)(this->WorldWidth >> 1);
	halfH = (float)(this->WorldHeight >> 1);

	x1 = x + (int)(((x - LightPosition.x) / halfW) * MaxShadowOffset.x);
	x2 = (x + width) + (int)(((x + width - LightPosition.x) / halfW) * MaxShadowOffset.x);
	finalWidth = x2 - x1;

	y1 = y + (int)(((y - LightPosition.y) / halfH) * MaxShadowOffset.y);
	y2 = (y + height) + (int)(((y + height - LightPosition.y) / halfH) * MaxShadowOffset.y);
	finalHeight = y2 - y1;

	this->BlitColoredFrameAbs(frame,
		(int)(x1 + this->OffsetX), (int)(y1 + this->OffsetY), 
		finalWidth, finalHeight,
		0.0f, 0.0f, 0.0f, 0.5f, false,
		flipX, flipY);
}

void Graphics::Clear()
{
	_gl->Clear();
}

void Graphics::DrawPolyLines(const vector<VECTOR2> &vertexes, float red, float green, float blue, float alpha) {
	vector<VECTOR2> translated;
	for (VECTOR2 v : vertexes) {
		translated.push_back(VECTOR2(v.x + (int)this->OffsetX, v.y + (int)this->OffsetY));
	}
	this->DrawPolyLinesAbs(translated, red, green, blue, alpha);
}

void Graphics::DrawString(int x, int y, const string &text, float rTop, float gTop, float bTop, float rBot, float gBot, float bBot) {
	this->_textRenderer.DrawStringAlpha(
		(int)(x + this->OffsetX), (int)(y + this->OffsetY),
		16,
		text, 
		rTop, gTop, bTop, 
		rBot, gBot, bBot, 
		1.0f);
}

void Graphics::DrawString(int x, int y, int textSize, const string &text, float rTop, float gTop, float bTop, float rBot, float gBot, float bBot) {
	this->_textRenderer.DrawStringAlpha(
		(int)(x + this->OffsetX), (int)(y + this->OffsetY),
		textSize,
		text, 
		rTop, gTop, bTop, 
		rBot, gBot, bBot, 
		1.0f);
}

void Graphics::DrawStringAlpha(int x, int y, const string &text, float rTop, float gTop, float bTop, float rBot, float gBot, float bBot, float alpha) {
	this->_textRenderer.DrawStringAlpha(
		(int)(x + this->OffsetX), (int)(y + this->OffsetY), 
		16, 
		text, 
		rTop, gTop, bTop, 
		rBot, gBot, bBot, 
		alpha);
}

void Graphics::DrawStringAlpha(int x, int y, int textSize, const string &text, float rTop, float gTop, float bTop, float rBot, float gBot, float bBot, float alpha) {
	this->_textRenderer.DrawStringAlpha(
		(int)(x + this->OffsetX), (int)(y + this->OffsetY),
		textSize,
		text, 
		rTop, gTop, bTop, 
		rBot, gBot, bBot, 
		alpha);
}

GLuint Graphics::CreateProgram(const vector<Shader*> shaders) {
	vector<GLuint> shaderIds;
	for (Shader* s : shaders) {
		shaderIds.push_back(s->ShaderId);
	}
	return this->_gl->CreateProgram(shaderIds);
}