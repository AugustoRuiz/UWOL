#pragma once

#include "../Common.h"
#include "Frame.h"
#include "../Input/Event.h"

#ifdef __APPLE__
#include "json.h"
#else
#include <json/json.h>
#endif

using namespace std;

class AnimationFrame
{
public:
	Frame* Current;
	unsigned int Duration;
	vector<Event> Events;

	AnimationFrame();
	~AnimationFrame();

	void Init(Json::Value &root);
};
