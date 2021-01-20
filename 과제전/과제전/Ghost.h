#pragma once
#include "DynamicObject.h"

class Ghost : public DynamicObject
{
private:

public:
	Ghost() {
		this->type = ObjectType::GHOST;
		this->position = Vector3(15,0,0);
	}
	virtual void DrawObject(GLuint s_program, GLuint VAO, int indexcCount) override;
};
