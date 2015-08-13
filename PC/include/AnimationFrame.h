#pragma once

#include "GameData.h"
#include "Frame.h"
#include "Event.h"
#include <json/json.h>

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
