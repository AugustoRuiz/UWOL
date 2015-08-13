#pragma once

#include <string>
#include <map>
#include <json/json.h>

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

