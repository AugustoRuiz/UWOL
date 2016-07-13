#include "Entities/Entity.h"

void Entity::AddComponent(string name, Component *value) {
	if(this->components.find(name) != this->components.end()) {
		Log::Out << "Component with name " << name << " already exists. Replacing." << endl ;
	}
	this->components[name] = value;
}

Component* Entity::RemoveComponent(string name) {
	Component* result = nullptr;
	COMPONENT_MAP::iterator it = this->components.find(name);
	if(it != this->components.end()) {
		result = it->second;
		this->components.erase(name);
	} 
	else {
		Log::Out << "No component found with key " << name << "." << endl;
	}
	return result;
}

Component* Entity::GetComponent(string name) {
	Component* result = nullptr;
	COMPONENT_MAP::iterator it = this->components.find(name);
	if(it != this->components.end()) {
		result = it->second;
	} else {
		Log::Out << "No component found with key " << name << "." << endl;
	}
	return result;
}

void Entity::SendMessage(Component* source, string message, void *data) {
	MESSAGE_REGISTER_MAP::iterator it = this->messageRegistration.find(message);
	if(it == this->messageRegistration.end()) {
		Log::Out << "No components registered for message " << message << "." << endl;
	} else {
		COMPONENTS destinationVector = this->messageRegistration[message];
		for(Component* c : destinationVector) {
			c->Receive(this, source, message, data);
		}
	}
}

void Entity::Update(int milliSecs) {
	for(COMPONENT_ENTRY p : this->components) {
		p.second->Update(this, milliSecs);
	}
}