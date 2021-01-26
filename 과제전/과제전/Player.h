#pragma once
#include "DynamicObject.h"
class Player : public DynamicObject
{
private:

public:
	Player();

	virtual void DrawObject(GLuint s_program) override;

	Vector3 GetPlayerPos() { return this->position; }
	void SetPlayerPos(Vector3 playerPos) { this->position = playerPos; }
	void SetPlayerPos(glm::vec3 playerPos) { this->position = playerPos; }
};

