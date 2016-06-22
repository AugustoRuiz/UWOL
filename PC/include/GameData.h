#pragma once

#include <vector>

#include <GL/glew.h>
#include <SDL2/SDL.h>
#include <GL/freeglut.h>

#ifdef __APPLE__
#include <SDL_image/SDL_image.h>
#include <SDL_mixer/SDL_mixer.h>
#include "CoreFoundation/CoreFoundation.h"
#else
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#endif

#include "UWOL.h"
#include "Log.h"

#define PI 3.14159265358979323846
#define TAU 2 * PI

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
