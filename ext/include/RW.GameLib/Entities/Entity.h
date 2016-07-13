#pragma once

#include <map>
#include <vector>
#include <string>
#include "Component.h"
#include "../Utils/Log.h"

using namespace std;

class Component;

typedef pair<string, Component*> COMPONENT_ENTRY;
typedef map<string, Component*> COMPONENT_MAP;

typedef vector<Component*> COMPONENTS;
typedef map<string, COMPONENTS> MESSAGE_REGISTER_MAP;
typedef pair<string, COMPONENTS> MESSAGE_REGISTER_ENTRY;

class Entity {
	COMPONENT_MAP components;
	MESSAGE_REGISTER_MAP messageRegistration;

public:
	void AddComponent(string name, Component *value);
	Component* RemoveComponent(string name);
	Component* GetComponent(string name);
	void SendMessage(Component *source, string message, void* data);

	void Update(int milliSecs);
};