#pragma once

#include "GameData.h"
#include <glm/glm.hpp>
#include <glm/ext.hpp>

class GLFuncs
{
public:
	static GLFuncs * GetInstance();

	glm::mat4 MVP;

	SDL_Window * Initialize(int screenWidth, int screenHeight, 
							 GLboolean fullscreen, const char* name);

	void ResetMVP();
	void OffsetMVP(float offsetX, float offsetY);

	void BlitVerts(float vertex_buffer_data[], unsigned int vBufSize, 
		float uv_buffer_data[], unsigned int uvBufSize, 
		float color_buffer_data[], unsigned int cBufSize);

	void BlitColoredRect(int iX, int iY, int width, int height,
					     float tx1, float ty1, float tx2, float ty2, float red,
						 float green, float blue, float alpha, bool additive);

	void BlitRect(int iX, int iY, int width, int height,
			      float tx1, float ty1, float tx2, float ty2);
	
	void BlitShadow(int iX, int iY, int width, int height,
					float tx1, float ty1, float tx2, float ty2);
	
	void BlitRect(int iX, int iY, int width, int height,
			      float tx1, float ty1, float tx2, float ty2, float alpha);

	void Clear();

	void DrawPolyLine(const vector<VECTOR2> &vertexes, float red, float green, float blue, float alpha);

	GLuint CreateProgram(const std::vector<GLuint> &shaderList);
	void UseProgram(GLuint programId);

	GLuint CreateShader(GLenum eShaderType, const std::string &strShaderFile);
	void DeleteShader(GLuint shaderId);

	void SwapBuffers();
	GLuint GetFramebufferTexture();

protected :
	GLFuncs(void){};
	virtual ~GLFuncs(void) {
		glDeleteTextures(1, &_renderedTexture);
		glDeleteFramebuffers(1, &_frameBufferName);
	};
	static GLFuncs _instance;

	void checkSDLError(int line);
	void setGLAttributes();
	bool initFramebuffer();

	bool _useFramebuffer;

	GLuint _frameBufferName;
	GLuint _renderedTexture;
	GLuint _quad_vertexbuffer;

	int _screenWidth;
	int _screenHeight;

	SDL_Window *_window;
	SDL_Renderer *_renderer;
	SDL_GLContext _mainContext;

	GLuint _vertexBuffer, _uvBuffer, _colorBuffer, _lineVertexBuffer;
};

