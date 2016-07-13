#pragma once

#include "../Common.h"
#include "../Graphics/IDrawable.h"
#include "../Entities/IUpdatable.h"
#include "AnimationFrame.h"

#ifdef __APPLE__
#include "json.h"
#else
#include <json/json.h>
#endif

using namespace std;

class Animation {
	static map<string, Animation*> Animations;

public:
	Animation();
	~Animation();

	string Name;
	vector<AnimationFrame*> Frames;
	bool Looped;

	void Init(Json::Value &root);
	void Dispose();

	static void LoadAnimations(const string &filePath);
	static Animation* Get(const string &name);
private:
	bool _disposed;
};

