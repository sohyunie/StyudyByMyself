#pragma once
#include "Object.h"

class DynamicObject : public Object
{
private:

public:
	DynamicObject() {

	}

	DIRECTION playerDirection = DIRECTION::DIR_NONE;
	DIRECTION progressDirection = DIRECTION::DIR_NONE;
};