#pragma once

#include "../Common.h"
#include "Animation.h"
#include "../Input/Event.h"

class AnimationPlayer {
	unsigned int _ticksSinceLastFrame;
	Animation *_animation;
	vector<AnimationFrame*>::iterator _current;
    vector<AnimationFrame*>::iterator _begin;
    vector<AnimationFrame*>::iterator _end;
public:
	AnimationPlayer();
	~AnimationPlayer();

	bool AnimationEnded;
	void setAnimation(Animation *animation);
	Frame* GetCurrentFrame();
	vector<Event> Update(Uint32 millisec);

	void setFrame(unsigned int frame);
	void setTicks(unsigned int ticks);
};

