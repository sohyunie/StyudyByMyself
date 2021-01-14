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
    PLAYER,
    GHOST,
    BEAD,
    POWERBEAD,
    BLOCK,
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
