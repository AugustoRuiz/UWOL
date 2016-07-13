#pragma once

#include <vector>
#include <RW.GameLib/RW.GameLib.h>
#include "UWOL.h"

#define ROOM_START_EVENT 0x10000
#define INERTIA_STATUS 0x20000

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

#ifndef _GLOBAL_VARS_
#define _GLOBAL_VARS_

extern bool scanlines;
extern bool debugPaint;

#endif
