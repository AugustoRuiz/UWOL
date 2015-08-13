#include "Animation.h"

Animation::Animation() {
}

Animation::~Animation() {
}

map<string, Animation> Animation::Animations;

void Animation::Init(Json::Value &root) {
	this->Name = root.get("name", "undefined").asString();
	this->Looped = root.get("loop", true).asBool();

	const Json::Value frames = root["frames"];
	if (frames != Json::Value::null) {
		for (Json::ValueIterator it = frames.begin(); it != frames.end(); ++it) {
			AnimationFrame frame;
			frame.Init(*it);
			this->Frames.push_back(frame);
		}
	}

	Animation::Animations[this->Name] = *this;
}

void Animation::LoadAnimations(const string &filePath) {
	ifstream file(filePath, ifstream::binary);
	Json::Value root;
	file >> root;

	if (!file.good()) {
		stringstream errorMsg;
		errorMsg << "El fichero '" << filePath << "' no se ha podido abrir correctamente";
		throw errorMsg.str();
	}

	if (root == Json::Value::null) {
		stringstream errorMsg;
		errorMsg << "No se ha podido leer correctamente el fichero '" << filePath << "'";
		throw errorMsg.str();
	}

	Json::Value animations = root["animations"];
	if (animations == Json::Value::null) {
		stringstream errorMsg;
		errorMsg << "No se ha encontrado la clave 'animations' en el fichero '" << filePath << "'";
		throw errorMsg.str();
	}

	for (Json::ValueIterator it = animations.begin(); it != animations.end(); ++it) {
		Animation anim;
		anim.Init(*it);
		Animation::Animations[anim.Name] = anim;
	}
}

Animation& Animation::Get(const string &name) {
	map<string, Animation>::iterator it = Animation::Animations.find(name);
	if (it != Animation::Animations.end()) {
		return it->second;
	}

	stringstream exceptionError;
	exceptionError << "Animation " << name << " not found!";
	throw exceptionError.str();
}
