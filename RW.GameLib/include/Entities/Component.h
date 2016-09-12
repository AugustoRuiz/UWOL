#pragma once

#include "../Common.h"
#include "Entity.h"
#include <string>

using namespace std;

class Entity;

class Component {

public:
	string Name;

	virtual void OnEnter();
	virtual void OnExit();
	virtual void Update(Entity *parent, Uint32 milliSec);
	virtual void Receive(Entity *parent, Component* source, string message, void* data);

};