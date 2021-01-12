#pragma once

struct Vector3 {
	float x;
	float y;
	float z;

	Vector3() {}
};

// 충돌처리 위한 벡터 구조체 
struct Vector4 {
    float minX;
    float minZ;
    float maxX;
    float maxZ;

    Vector4() {}

    Vector4(float minX, float minZ, float maxX, float maxZ) {
        this->minX = minX;
        this->minZ = minZ;
        this->maxX = maxX;
        this->maxZ = maxZ;
    }
};

enum ObjectType {
    BEAD,
    BLOCK,
    GHOST,
    PLAYER,
    POWERBEAD,
};
