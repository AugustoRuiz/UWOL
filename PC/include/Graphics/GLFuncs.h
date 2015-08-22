#pragma once

#include "GameData.h"

class GLFuncs
{
public:
	static GLFuncs * GetInstance();
	
	SDL_Surface * Initialize(int screenWidth, int screenHeight, 
							 GLboolean fullscreen);

	void BlitColoredRect(GLuint texNumber, int iX, int iY, int width, int height,
					     float tx1, float ty1, float tx2, float ty2, GLfloat red,
						 GLfloat green, GLfloat blue, GLfloat alpha, bool additive);

	void BlitRect(GLuint texNumber, int iX, int iY, int width, int height,
			      float tx1, float ty1, float tx2, float ty2);
	
	void BlitShadow(GLuint texNumber, int iX, int iY, int width, int height,
					float tx1, float ty1, float tx2, float ty2);
	
	void BlitRect(GLuint texNumber, int iX, int iY, int width, int height,
			      float tx1, float ty1, float tx2, float ty2, float alpha);

	void DrawPolyLine(const vector<VECTOR2> &vertexes, float red, float green, float blue, float alpha);

protected :
	GLFuncs(void){};
	virtual ~GLFuncs(void) {};
	static GLFuncs _instance;

};

