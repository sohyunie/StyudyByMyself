#pragma once
#include "Object.h"
class StaticObject : public Object
{
private:

public:
	StaticObject() {

	}
	virtual void DrawObject(GLuint s_program, GLuint VAO, int indexCount) override {
		//cout << "DrawObject : StaticObject" << endl;
	}
};

