#pragma once
#include "DynamicObject.h"
class Player : public DynamicObject
{
private:

public:
	Player() {
		this->type = ObjectType::PLAYER;
		this->position = Vector3(20, 20, 0);
	}

	virtual void DrawObject(GLuint s_program) override;

	Vector3 GetPlayerPos() { return this->position; }
	void SetPlayerPos(Vector3 playerPos) { this->position = playerPos; }
};

