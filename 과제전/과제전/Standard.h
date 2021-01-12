#pragma once

struct Vector3 {
	float x;
	float y;
	float z;

	Vector3() {}
};

// �浹ó�� ���� ���� ����ü 
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
