#pragma once
class Block;    // 기능은 못쓰고 Block이라는 애가 존재해만 알 수 있음
class Ghost;

class InGameManager
{
private:
	static InGameManager* instance; // *쓰는 이유: 하나만 존재하게 하려고 / * 이거 안쓰면 메모리가 잡혀버리고 시작. 동적할당은 그 때 그때 메모리를 새로 할당/ 메모리 낭비 방지하려는 차원/
    // static으로 메모리 잡히면? 프로그램 컴파일 했을 때 즉시로 메모리가 잡힘 / 그냥 변수 쓰면 저 변수를 호출할 때 잡힘
    // static을 사용하는 이유: 프로그램 내에서 메모리를 옮길 필요없는 변수라서, 프로그램 내에서 메모리를 한번만 차지하면 되서?
    GLuint VAO[MAX_VAO_TYPE];
    GLuint VBO[MAX_VAO_TYPE][3];
    GLuint EBO[MAX_VAO_TYPE];
    //vector<Block> vBlock; // 여기서 변수 저렇게 써있자나..?! ㅇ그러면 메모리가 ㅇ우ㅛㅇ렁훙겡 웅웅ㅇ 요렇게 잡혀 그런데우웅
    Block* block;
    Block* block2;
    Ghost* ghost;
    ObjData* gobj;
    glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, -25.0f);
    glm::vec3 cameraDirection = glm::vec3(0.0f, 0.0f, 1.0f);
    glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
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
    glm::vec3 GetCameraPos() { return this->cameraPos; }
    glm::vec3 GetCameraDirection() { return this->cameraDirection; }
    glm::vec3 GetCameraUp() { return this->cameraUp; }
    void SetCamera(glm::vec3 camera) { this->cameraPos = camera; }; // set은 void / return타입이 없어도됨
protected:

};