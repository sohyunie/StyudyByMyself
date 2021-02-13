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
	void InitPlayerPos(int i, int j, Vector3 pos);
	void ComputePos(float deltaMove, float lx, float lz);
	float angle = 0.0f;
	float deltaAngle = 0.0f;
	float deltaMove = 0.0f;
	float hp = 100.0f;
	int board_i = 3;
	int board_j = 2;
	DIRECTION playerDirection = DIRECTION::DIR_NONE;
	Vector3 playerPriorDirVec3;
	Vector3 playerDirVec3;
};


