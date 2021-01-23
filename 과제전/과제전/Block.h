#pragma once
#include "StaticObject.h"

class Block : public StaticObject
{
private:
public:
	Block() {
		this->type = ObjectType::WALL;
	}
	Block(Vector3 pos) {
		this->type = ObjectType::WALL;
		this->position = pos; 
	}

	virtual void DrawObject(GLuint s_program) override;
};
