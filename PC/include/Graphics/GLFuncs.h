#pragma once

#include "GameData.h"
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <string>
#include <sstream>
#include <map>

using glm::vec2;
using glm::vec3;
using glm::vec4;
using glm::mat4;
using glm::mat3;

class GLFuncs
{
public:
	static GLFuncs * GetInstance();

	glm::mat4 MVP;

	SDL_Window * Initialize(int screenWidth, int screenHeight, 
							 GLboolean fullscreen, const char* name);

	void ResetMVP();
	void OffsetMVP(float offsetX, float offsetY);

	void SetTexture(unsigned int channel, unsigned int texture);

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

	void SetUniform(GLuint programId, const string &name, float x, float y, float z);
	void SetUniform(GLuint programId, const string &name, const vec2 & v);
	void SetUniform(GLuint programId, const string &name, const vec3 & v);
	void SetUniform(GLuint programId, const string &name, const vec4 & v);
	void SetUniform(GLuint programId, const string &name, const mat4 & m);
	void SetUniform(GLuint programId, const string &name, const mat3 & m);
	void SetUniform(GLuint programId, const string &name, float val);
	void SetUniform(GLuint programId, const string &name, int val);
	void SetUniform(GLuint programId, const string &name, bool val);
	void SetUniform(GLuint programId, const string &name, GLuint val);

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
	vector<unsigned int> _activeTextures;

	static GLFuncs _instance;

	map<int, map<string, int>> _uniformLocationCache;
	GLuint getUniformLocation(GLuint programId, const std::string &uniformName);

	void checkSDLError(int line);
	string getErrorString(GLuint error);
	void setGLAttributes();
	bool initFramebuffer();

	bool _useVBO;
	bool _useFramebuffer;

	GLuint _frameBufferName;
	GLuint _renderedTexture;
	GLuint _quad_vertexbuffer;

	GLuint _vtxAttribIdx, _uvAttribIdx, _colAttribIdx;

	GLuint _currentProgram;

	int _screenWidth;
	int _screenHeight;

	SDL_Window *_window;
	SDL_Renderer *_renderer;
	SDL_GLContext _mainContext;

	GLuint _vaoVertex;
	GLuint _vboVertex, _vboUV, _vboColor, _vboLineVertex;
};

