#pragma once

#include "../Common.h"
#include "../Graphics/IDrawable.h"
#include "IUpdatable.h"
#include "../Graphics/Graphics.h"
#include "../Graphics/TextRenderer.h"

class MessageLine : public IDrawable, public IUpdatable
{
public:
	MessageLine();
	~MessageLine();

	void ShowText(const string &text, Uint32 msecs, vec3 &rgbT, vec3 &rgbB);

	// Inherited via IDrawable
	virtual void Draw(void) override;
	virtual void DrawShadow(void) override;
	virtual bool DrawWhenNoCoins(void) override;

	// Inherited via IUpdatable
	virtual void Update(Uint32 milliSec) override;
	virtual bool UpdateWhenNoCoins(void) override;

private:
	TextRenderer* _textRender;

	int _remainingTime, _textSize, _posY, _posX;
	float _textAlpha;
	float _alphaIncr;
	vec3 _rgbT, _rgbB;
	std::string _text;
};

