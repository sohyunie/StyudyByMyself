#pragma once
class Block;    // 기능은 못쓰고 Block이라는 애가 존재해만 알 수 있음
class Ghost;
class Player;
class MapLoader;
class Bead;
class PowerBead;
class Object;
class InGameUI;

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
    Ghost* ghost[20];
    Object* object;
    ObjData* objData[MAX_VAO_TYPE];
    Player* player;
    MapLoader* map;
    Bead* bead;
    PowerBead* powerBead;
    InGameUI* ingameUI;
    glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
    glm::vec3 cameraDirection = glm::vec3(0.0f, 0.0f, 0.25f); 
    glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
    glm::vec3 offset = glm::vec3(0.0f, 15.5f, 0.0f); // 카메라 오차범위계산 눈은 위에 달림
    
    float degreeCameraRotate = 45.0f; // 요고야 요고 근데 얘 안쓰고 있을걸?  
    float degreeLightPos = 0.0f;
    float deltaTime = 0.0f;
    float lastFrame = 0.0f;
    float currentFrame = 0.0f;
    float additonalTime = 0.0f;
    float inGameTime = 0.0f;
    float angle, lx, lz = 0.0f;
    float ghostSpawnRunningTime = 0.0f;
    glm::vec3 ghostColor = glm::vec3(0.0f, 0.0f, 0.0f);
    int beadNumber = 0;
    bool isDrawFill = true;
    bool isFPS = false;
    bool isGhost = false;
    bool isBead = true;
    bool EatBead = false;
    bool CollideBead = false;
    bool isPowerBead = false;
    bool isInitComplete = false;
    bool isChangeCameraDir = false;
    bool isArrived = true; // 플레이어가 다른 칸에 도착한 그 순간! true, 그 외에는 항상 

    float acc = 0.f; // 이동 amount
    float accDir = 0.f; // 회전 amount
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
    float GetDeltaTime() { return this->deltaTime; }
    float GetIngameTime() { return this->inGameTime; }
    float currentTime() { return MAX_TIME - round(this->inGameTime / 100) / 10 + this->additonalTime; }
    float GetTime();
    int GetBeadNumber() { return this->beadNumber; }
    bool GetPresence() { return this->isBead; }
    void CalculateTime();
    void CameraSetting();
    void TimerFunction();
    void CheckDirection(bool isCollision, int i, int j);
    Vector3 DirToVec3(DIRECTION dir);

    bool GetIsDrawFill() { return this->isDrawFill; }
    glm::vec3 GetCameraPos() { return this->cameraPos; }
    glm::vec3 GetCameraDirection() { return this->cameraDirection; }
    glm::vec3 GetCameraUp() { return this->cameraUp; }
    void SetCameraPos(glm::vec3 camera) { this->cameraPos = camera + offset; }; // set은 void / return타입이 없어도됨
    void SetDegreeCameraRotate(float rotate) { this->degreeCameraRotate = rotate; }
    void SetCameraDirection(glm::vec3 dir) {   this->cameraDirection = dir; }
    void SetFPS(bool isOn) { this->isFPS = isOn; }
    Ghost* GetGhost();

    Player* GetPlayer() { return this->player; }    // GM에서 player를 불러서 사용하고 싶으니까 여기서 getplayer를 만들어서 한 싱글턴 구조 안에서 player불러서 사용할 수 있게 함
    MapLoader* LoadMap() { return this->map; }
    InGameUI* GetInGameUI() { return this->ingameUI; }
    GLvoid DrawMap(){}
    bool GetFPS() { return this->isFPS; }

    GLint GetVAO(ObjectType type) { return this->VAO[type]; }
    ObjData* GetObjData(ObjectType type) { return this->objData[type]; }

    Vector3 dir;
protected:

};