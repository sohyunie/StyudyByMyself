#pragma once
#include "StaticObject.h"

class Block : public StaticObject
{
private:
public:
	Block() {
		this->type = ObjectType::BLOCK;
	}
	Block(Vector3 pos) {
		this->type = ObjectType::BLOCK;
		this->position = pos; 
	}

	virtual void DrawObject(GLuint s_program, GLuint VAO, int indexCount) override;
};
