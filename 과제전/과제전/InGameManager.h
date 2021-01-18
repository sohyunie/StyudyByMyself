#pragma once
class Block;    // 기능은 못쓰고 Block이라는 애가 존재해만 알 수 있음
class Ghost;

class InGameManager
{
private:
	static InGameManager* instance;
    GLuint VAO[MAX_VAO_TYPE];
    GLuint VBO[3][MAX_VAO_TYPE];
    GLuint EBO[MAX_VAO_TYPE];
    //vector<Block> vBlock;
    Block* block;
    Ghost* ghost;
    ObjData* gobj;
    float degreeCameraRotate = 0.0f;
    float degreeLightPos = 0.0f;
    bool isDrawFill = true;

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

    float GetDegreeCameraRotate() { return this->degreeCameraRotate; }
    float GetDegreeLightPos() { return this->degreeLightPos; }
    bool GetIsDrawFill() { return this->isDrawFill; }
protected:

};