#include "Entities/MessageLine.h"

#define ALPHA_INCR 0.02f

MessageLine::MessageLine(){
	this->_textAlpha = 0.0f;
	this->_alphaIncr = 0.0f;
	this->_textRender = new TextRenderer();
	this->_textSize = 16;
	this->_remainingTime = 0;
	Graphics* g = Graphics::GetInstance();

	this->_posX = (Uint32) (g->OffsetX);
	this->_posY = (Uint32) (g->WorldHeight + (g->ScreenHeight - g->WorldHeight) / 2.0f + (2 * this->_textSize));
}

MessageLine::~MessageLine(){
}

void MessageLine::ShowText(const string & text, Uint32 msecs, vec3 &rgbT, vec3 &rgbB) {
	this->_alphaIncr = ALPHA_INCR;
	this->_remainingTime = msecs;
	this->_text = text;
	this->_rgbT = rgbT;
	this->_rgbB = rgbB;
}

void MessageLine::Draw(void) {
	if (this->_textAlpha != 0) {
		this->_textRender->DrawStringAlpha(
			_posX, _posY,
			this->_textSize, this->_text,
			this->_rgbB.r, this->_rgbB.g, this->_rgbB.b,
			this->_rgbT.r, this->_rgbT.g, this->_rgbT.b,
			this->_textAlpha);
	}
}

void MessageLine::DrawShadow(void) {
	if (this->_textAlpha != 0) {
		this->_textRender->DrawStringAlpha(
			_posX + 2, _posY + 2,
			this->_textSize, this->_text,
			0.0f, 0.0f, 0.0f,
			0.0f, 0.0f, 0.0f,
			0.5f * this->_textAlpha);
	}
}

bool MessageLine::DrawWhenNoCoins(void) {
	return true;
}

void MessageLine::Update(Uint32 milliSec, const Event& inputEvent) {
	if (_alphaIncr != 0.0f) {
		this->_textAlpha += _alphaIncr;
		this->_textAlpha = glm::clamp(this->_textAlpha, 0.0f, 1.0f);
	}

	if (this->_textAlpha == 1.0f) {
		this->_alphaIncr = 0.0f;
		this->_remainingTime -= milliSec;
		if (this->_remainingTime < 0) {
			this->_remainingTime = 0;
			this->_alphaIncr = -ALPHA_INCR;
		}
	}
}

bool MessageLine::UpdateWhenNoCoins(void) {
	return true;
}
