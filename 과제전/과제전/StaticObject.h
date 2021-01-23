#pragma once
#include "Object.h"
class StaticObject : public Object
{
private:

public:
	StaticObject() {

	}
	virtual void DrawObject(GLuint s_program) override {
		//cout << "DrawObject : StaticObject" << endl;
	}
};

