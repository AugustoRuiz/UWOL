#pragma once

#include "../Common.h"

struct IChaseable
{
public:
	virtual bool ShouldChase() = 0;
	virtual VECTOR2F GetPosition() = 0;
};