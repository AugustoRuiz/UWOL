#pragma once

#include <vector>

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

#include "UWOL.h"
#include "Log.h"

#define PI 3.14159265358979323846
#define TAU 2 * PI

#define ROOM_START_EVENT 0x10000

#define COLLISION_TYPE_NONE 0
#define COLLISION_TYPE_UP 1
#define COLLISION_TYPE_DOWN 2
#define COLLISION_TYPE_LEFT 4
#define COLLISION_TYPE_RIGHT 8

#define COLLISION_NONE      0
#define COLLISION_BLOCK     1
#define COLLISION_PLATFORM  2
#define COLLISION_STAIRS    4

#define KEY_UP     0
#define KEY_DOWN   1
#define KEY_LEFT   2
#define KEY_RIGHT  3

#define MAP_COLS 12
#define MAP_ROWS 10

#define TICKS_PER_FRAME 17 // (1000 mSec / 100 frames)

#define EVENT_BUFFER_MAX_SIZE 10000

#define PIX_PER_MILLISEC 0.054f //0.0006f
#define ACCEL_FANTY 0.005f //0.30f

#define TICKS_ANIM_UWOL 80
#define TICKS_ANIM_ENEM 160
#define TICKS_ANIM_COIN 160
#define TICKS_PARPADEO 40
#define TICKS_INVULNERABLE 1000
#define TICKS_SALTO 240

#define FADE_STEP 0.002f

typedef int CollisionType;

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

typedef struct textureInfo
{
	GLuint texture;
	int width;
	int height;
} TEXTUREINFO;

typedef struct textureRect
{
	float tx1;
	float tx2;
	float ty1;
	float ty2;
} TEXTURERECT;

#ifndef _GLOBAL_VARS_
#define _GLOBAL_VARS_

extern bool aliasing;
extern bool scanlines;
extern bool debugPaint;

#endif
