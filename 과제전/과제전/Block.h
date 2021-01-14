#pragma once
#include "StaticObject.h"

class Block : public StaticObject
{
private:

public:
	Block(Vector3 pos) {
		this->position = pos;
		this->type = ObjectType::BLOCK;
	}

	virtual void DrawObject(GLuint s_program, GLuint VAO) override;

};
