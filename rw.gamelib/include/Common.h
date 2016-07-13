#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>

#ifdef __APPLE__
#include "CoreFoundation/CoreFoundation.h"
#if TARGET_IOS
#import <OpenGLES/ES2/gl.h>
#import <OpenGLES/ES2/glext.h> 
#else
#import <OpenGL/OpenGL.h>
// OpenGL 3.2 is only supported on MacOS X Lion and later
// CGL_VERSION_1_3 is defined as 1 on MacOS X Lion and later
#if CGL_VERSION_1_3
// Set to 0 to run on the Legacy OpenGL Profile
#define ESSENTIAL_GL_PRACTICES_SUPPORT_GL3 1
#else
#define ESSENTIAL_GL_PRACTICES_SUPPORT_GL3 0
#endif //!CGL_VERSION_1_3

#if ESSENTIAL_GL_PRACTICES_SUPPORT_GL3
#import <OpenGL/gl3.h>
#else
#import <OpenGL/gl.h>
#endif //!ESSENTIAL_GL_PRACTICES_SUPPORT_GL3

#endif // !TARGET_IOS

//The name of the VertexArrayObject are slightly different in
// OpenGLES, OpenGL Core Profile, and OpenGL Legacy
// The arguments are exactly the same across these APIs however
#if TARGET_IOS
#define glBindVertexArray glBindVertexArrayOES
#define glGenVertexArrays glGenVertexArraysOES
#define glDeleteVertexArrays glDeleteVertexArraysOES
#else
#if ESSENTIAL_GL_PRACTICES_SUPPORT_GL3
#define glBindVertexArray glBindVertexArray
#define glGenVertexArrays glGenVertexArrays
#define glGenerateMipmap glGenerateMipmap
#define glDeleteVertexArrays glDeleteVertexArrays
#else
#define glBindVertexArray glBindVertexArrayAPPLE 
#define glGenVertexArrays glGenVertexArraysAPPLE
#define glGenerateMipmap glGenerateMipmapEXT
#define glDeleteVertexArrays glDeleteVertexArraysAPPLE
#endif //!ESSENTIAL_GL_PRACTICES_SUPPORT_GL3
#endif //!TARGET_IOS

#else //!APPLE
#include <GL/glew.h>
#include <SDL2/SDL_opengl.h>
#endif

#define PI 3.14159265358979323846
#define TAU 2 * PI

typedef struct rect
{
	int x;
	int y;
	int width;
	int height;
} RECTANGLE;

typedef struct rectF
{
	float x;
	float y;
	float width;
	float height;
} RECTANGLEF;

typedef struct vector2
{
	int x;
	int y;
	vector2() : x(0), y(0) {};
	vector2(int vx, int vy) : x(vx), y(vy) {};
} VECTOR2;

typedef struct vector2f
{
	float x;
	float y;
	vector2f() : x(0.0f), y(0.0f) {};
	vector2f(float vx, float vy) : x(vx), y(vy) {};
} VECTOR2F;

typedef struct textureRect
{
	float tx1;
	float tx2;
	float ty1;
	float ty2;
} TEXTURERECT;

typedef struct textureInfo
{
	GLuint texture;
	int width;
	int height;
	TEXTURERECT texCoords;
} TEXTUREINFO;
