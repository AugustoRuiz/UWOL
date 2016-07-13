#pragma once

#include <string>
#include <map>

#ifdef __APPLE__
#include "json.h"
#else
#include <json/json.h>
#endif

using namespace std;

class Event
{
public:
	Event();
	~Event();

	string Name;
	map<string, Json::Value> Data;

	void Init(Json::Value root);
};

