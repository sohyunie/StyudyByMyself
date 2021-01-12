#pragma once
#include "DynamicObject.h"

class Ghost : public DynamicObject
{
private:

public:
	Ghost() {
		this->type = ObjectType::GHOST;
	}
	void FindRoad() {

	}
};
