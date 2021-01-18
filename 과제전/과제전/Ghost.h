#pragma once
#include "DynamicObject.h"

class Ghost : public DynamicObject
{
private:

public:
	Ghost() {
		this->type = ObjectType::GHOST;
	}
	virtual void DrawObject(GLuint s_program, GLuint VAO, int indexcCount) override;
};
