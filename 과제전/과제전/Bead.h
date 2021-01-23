#pragma once
#include "StaticObject.h"
class Bead : public StaticObject
{
private:

public:
	Bead() {
		this->type = ObjectType::BEAD;
	}
	Bead(Vector3 position) {
		this->position = position;
	}
	virtual void DrawObject(GLuint s_program, GLuint VAO, int indexcCount) override;
};
