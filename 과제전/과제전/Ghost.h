#pragma once
#include "DynamicObject.h"

class Ghost : public DynamicObject
{
private:

public:
	Ghost();
	Ghost(Vector3 pos);
	virtual void DrawObject(GLuint s_program) override;
};
