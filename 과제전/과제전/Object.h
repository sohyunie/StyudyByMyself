#pragma once
#include <iostream>
#include "Standard.h"
using namespace std;

class Object
{
private:

public:
	Object() { }
	virtual void DrawObject();
	bool CollisionCheck(Object type);
	Vector4 GetBoundingBox();
	Vector3 GetPosition() { return position; }
protected:
	Vector3 position;
	Vector3 scale;
	ObjectType type;
	float radius; // bbox¿ß«‘!
};