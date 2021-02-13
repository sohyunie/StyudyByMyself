#pragma once
#include "Object.h"

class DynamicObject : public Object
{
private:

public:
	DynamicObject() {

	}

	DIRECTION progressDirection = DIRECTION::DIR_NONE;
	DIRECTION priorDirection = DIRECTION::DIR_NONE;
};