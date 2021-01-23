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
		this->position = position;
	}
	virtual void DrawObject(GLuint s_program, GLuint VAO, int indexcCount) override;
};

