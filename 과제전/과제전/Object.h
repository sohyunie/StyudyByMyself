#pragma once
#ifndef OBJECT_H
#define OBJECT_H

class Object
{
private:

public:
	Object() { }
	virtual void DrawObject(GLuint s_program, GLuint VAO, int indexCount);
	bool CollisionCheck(Object type);
	Vector4 GetBoundingBox();
	Vector3 GetPosition() { return position; }
protected:
	Vector3 position;
	Vector3 scale;
	ObjectType type;
	float radius; // bbox����!
};

#endif