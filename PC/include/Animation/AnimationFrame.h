#pragma once

#include "GameData.h"
#include "Frame.h"
#include "Event.h"

#ifdef __APPLE__
#include "json.h"
#else
#include <json/json.h>
#endif

using namespace std;

class AnimationFrame
{
public:
	Frame Current;
	unsigned int Duration;
	vector<Event> Events;

	AnimationFrame();
	~AnimationFrame();

	void Init(Json::Value &root);
};
