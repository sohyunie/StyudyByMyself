#pragma once
#include "StaticObject.h"
class PowerBead : public StaticObject
{
private:

public:
	PowerBead() {
		this->type = ObjectType::POWERBEAD;
	}
	PowerBead(Vector3 position) {
		this->type = ObjectType::POWERBEAD;
		this->position = position;
	}
	virtual void DrawObject(GLuint s_program) override;
};

