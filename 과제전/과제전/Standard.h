#pragma once

#pragma warning(disable:4996)
#include <iostream>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <random>
#include <time.h>
#include <windows.h>
#include <vector>
#include <gl/glew.h> 
#include <gl/freeglut.h>
#include <gl/freeglut_ext.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace std;
const int MAX_VAO_TYPE = 5;

#define WINDOW_WITDH	800
#define WINDOW_HEIGHT	600

#define CAMERA_POS		glm::vec3(0.0, 0.0, 50.0)
#define OBJECT_COLOR	glm::vec3(1.0, 1.0, 1.0)
#define LIGHT_AMBIENT	glm::vec3(0.1, 0.1, 0.1)
#define LIGHT_POS		glm::vec3(1000.0, 1000.0, 100.0)
#define LIGHT_COLOR		glm::vec3(1.0, 1.0, 1.0)

#define FILE_NAME		"sphere.obj"


struct ObjData {
	float* vPosData;	// 값이 하나만 있어도 되는 건 그냥 변수로/ 아니고 여러개가 나열되고 필요한 것들은 *로 받는다.
	float* vNormalData;
	float* vTextureCoordinateData;
	int* indexData;
	int vertexCount;
	int indexCount;
};

struct Vector3 {
	float x;
	float y;
	float z;

	Vector3(float x, float y, float z) {
		this->x = x;
		this->y = y;
		this->z = z;
	}
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
    PLAYER,
	BLOCK,
    BEAD,
	GHOST,
    POWERBEAD,
};


const float blockVertex[6][6][2][3] = {
	//--- 버텍스 속성: 좌표값(FragPos), 노말값 (Normal)

	{
		// Back
		{	{-0.5f, -0.5f, -0.5f},	{0.0f, 0.0f, -1.0f}	},
		{	{ 0.5f, -0.5f, -0.5f},	{0.0f, 0.0f, -1.0f}	},
		{	{ 0.5f,  0.5f, -0.5f},	{0.0f, 0.0f, -1.0f}	},
		{	{ 0.5f,  0.5f, -0.5f},	{0.0f, 0.0f, -1.0f}	},
		{	{-0.5f,  0.5f, -0.5f},	{0.0f, 0.0f, -1.0f}	},
		{	{-0.5f, -0.5f, -0.5f},	{0.0f, 0.0f, -1.0f}	}
	},

	{
		// Front
		{	{-0.5f, -0.5f, 0.5f},	{0.0f, 0.0f, 1.0f}	},
		{	{ 0.5f, -0.5f, 0.5f},	{0.0f, 0.0f, 1.0f}	},
		{	{ 0.5f,  0.5f, 0.5f},	{0.0f, 0.0f, 1.0f}	},
		{	{ 0.5f,  0.5f, 0.5f},	{0.0f, 0.0f, 1.0f}	},
		{	{-0.5f,  0.5f, 0.5f},	{0.0f, 0.0f, 1.0f}	},
		{	{-0.5f, -0.5f, 0.5f},	{0.0f, 0.0f, 1.0f}	}
	},

	{
		// Left
		{	{-0.5f,  0.5f,  0.5f},	{-1.0f, 0.0f, 0.0f}	},
		{	{-0.5f,  0.5f, -0.5f},	{-1.0f, 0.0f, 0.0f}	},
		{	{-0.5f, -0.5f, -0.5f},	{-1.0f, 0.0f, 0.0f}	},
		{	{-0.5f, -0.5f, -0.5f},	{-1.0f, 0.0f, 0.0f}	},
		{	{-0.5f, -0.5f,  0.5f},	{-1.0f, 0.0f, 0.0f}	},
		{	{-0.5f,  0.5f,  0.5f},	{-1.0f, 0.0f, 0.0f}	}
	},

	{
		// Right
		{	{0.5f,  0.5f,  0.5f},	{1.0f, 0.0f, 0.0f}	},
		{	{0.5f,  0.5f, -0.5f},	{1.0f, 0.0f, 0.0f}	},
		{	{0.5f, -0.5f, -0.5f},	{1.0f, 0.0f, 0.0f}	},
		{	{0.5f, -0.5f, -0.5f},	{1.0f, 0.0f, 0.0f}	},
		{	{0.5f, -0.5f,  0.5f},	{1.0f, 0.0f, 0.0f}	},
		{	{0.5f,  0.5f,  0.5f},	{1.0f, 0.0f, 0.0f}	}
	},
	{
		// Bottom
		{	{-0.5f, -0.5f, -0.5f},	{0.0f, -1.0f, 0.0f}	},
		{	{ 0.5f, -0.5f, -0.5f},	{0.0f, -1.0f, 0.0f} },
		{	{ 0.5f, -0.5f,  0.5f},	{0.0f, -1.0f, 0.0f}	},
		{	{ 0.5f, -0.5f,  0.5f},	{0.0f, -1.0f, 0.0f}	},
		{	{-0.5f, -0.5f,  0.5f},	{0.0f, -1.0f, 0.0f}	},
		{	{-0.5f, -0.5f, -0.5f},	{0.0f, -1.0f, 0.0f}	}
	},

	{
		// Top
		{	{-0.5f, 0.5f, -0.5f},	{0.0f, 1.0f, 0.0f}	},
		{	{ 0.5f, 0.5f, -0.5f},	{0.0f, 1.0f, 0.0f}	},
		{	{ 0.5f, 0.5f,  0.5f},	{0.0f, 1.0f, 0.0f}	},
		{	{ 0.5f, 0.5f,  0.5f},	{0.0f, 1.0f, 0.0f}	},
		{	{-0.5f, 0.5f,  0.5f},	{0.0f, 1.0f, 0.0f}	},
		{	{-0.5f, 0.5f, -0.5f},	{0.0f, 1.0f, 0.0f}	}
	}
};
