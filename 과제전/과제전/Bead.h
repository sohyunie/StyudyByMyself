#pragma once
#include "StaticObject.h"
class Bead : public StaticObject
{
private:

public:
	Bead() {
		this->type = ObjectType::BEAD;
	}
	void EatBead() {

	}
};
