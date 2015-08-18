#include "Event.h"

Event::Event()
{
}


Event::~Event()
{
}

void Event::Init(Json::Value root)
{
	this->Name = root.get("name", "undefined").asString();
	Json::Value dataArray = root["data"];
	if (dataArray != Json::Value::null) {
		for (Json::ValueIterator it = dataArray.begin(); it != dataArray.end(); ++it) {
			Json::Value currentData = *it;
			string dataKey = currentData.get("name", "undefined").asString();
			Json::Value dataValue = currentData["value"];
			this->Data[dataKey] = dataValue;
		}
	}
}
