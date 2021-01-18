#pragma once
#include "StaticObject.h"

class Block : public StaticObject
{
private:

public:
	Block() {
		this->type = ObjectType::BLOCK;
	}

	virtual void DrawObject(GLuint s_program, GLuint VAO, int indexCount) override;
};
