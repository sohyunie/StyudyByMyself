#pragma once
#include "StaticObject.h"
class PowerBead : public StaticObject
{
private:

public:
	PowerBead() {
		this->type = ObjectType::POWERBEAD;
	}

	void EatPowerBead() {

	}
};

