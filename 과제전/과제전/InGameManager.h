#pragma once
#include "Standard.h"
#include "Block.h"

class InGameManager
{
private:
	static InGameManager* instance;
    GLuint VAO[MAX_VAO_TYPE];
    GLuint VBO[MAX_VAO_TYPE];
    vector<Block> vBlock;

public:
    static InGameManager& GetInstance() {
        if (instance == NULL) {
            instance = new InGameManager();
        }
        return *instance;
    }

	GLvoid DrawScene();
    GLvoid InitBuffer();
    GLvoid InitShader();
    GLvoid InitObject();
protected:

};