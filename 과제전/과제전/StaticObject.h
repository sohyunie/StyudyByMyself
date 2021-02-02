#pragma once
#include "Object.h"
class StaticObject : public Object
{
private:

public:
	StaticObject() {
		this->type = ObjectType::ROAD;
	}
	virtual void DrawObject(GLuint s_program) override {
		//cout << "DrawObject : StaticObject" << endl;
	}
};