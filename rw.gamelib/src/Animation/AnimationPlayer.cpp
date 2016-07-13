#include "Animation/AnimationPlayer.h"

AnimationPlayer::AnimationPlayer() {
	this->_animation = NULL;
}

AnimationPlayer::~AnimationPlayer()
{
}

void AnimationPlayer::setAnimation(Animation *animation) {
	if ((NULL == this->_animation) || (this->_animation->Name != animation->Name)) {
		this->_animation = animation;
        this->_begin = animation->Frames.begin();
        this->_end = animation->Frames.end();
		this->_current = this->_begin;
		this->_ticksSinceLastFrame = 0;
	}
}

vector<Event> AnimationPlayer::Update(Uint32 milliSec) {
	vector<Event> result;
	this->AnimationEnded = false;
	this->_ticksSinceLastFrame += milliSec;

	while (this->_ticksSinceLastFrame >= (*this->_current)->Duration) {
		this->_ticksSinceLastFrame -= (*this->_current)->Duration;
		++this->_current;
		if (this->_current == this->_end) {
			if (this->_animation->Looped) {
				this->_current = this->_begin;
			}
			else {
				--this->_current;
			}
			this->AnimationEnded = true;
		}
	}
	return result;
}

Frame* AnimationPlayer::GetCurrentFrame() {
	return (*this->_current)->Current;
}

void AnimationPlayer::setFrame(unsigned int frame) {
	this->_current = this->_animation->Frames.begin() + frame;
}

void AnimationPlayer::setTicks(unsigned int ticks) {
	this->_ticksSinceLastFrame = ticks;
}
