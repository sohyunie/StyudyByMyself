#pragma once
class Block;    // ����� ������ Block�̶�� �ְ� �����ظ� �� �� ����
class Ghost;
class Player;
class MapLoader;
class Bead;
class PowerBead;
class Object;
class InGameUI;
class DynamicObject;
class Bottom;
class StartSceneUI;
class EndingScene;

#include <algorithm>
#include <list>

class InGameManager
{
private:
	static InGameManager* instance; // *���� ����: �ϳ��� �����ϰ� �Ϸ��� / * �̰� �Ⱦ��� �޸𸮰� ���������� ����. �����Ҵ��� �� �� �׶� �޸𸮸� ���� �Ҵ�/ �޸� ���� �����Ϸ��� ����/
    // static���� �޸� ������? ���α׷� ������ ���� �� ��÷� �޸𸮰� ���� / �׳� ���� ���� �� ������ ȣ���� �� ����
    // static�� ����ϴ� ����: ���α׷� ������ �޸𸮸� �ű� �ʿ���� ������, ���α׷� ������ �޸𸮸� �ѹ��� �����ϸ� �Ǽ�?
    GLuint VAO[MAX_VAO_TYPE];
    GLuint VBO[MAX_VAO_TYPE][3];
    GLuint EBO[MAX_VAO_TYPE];
    //vector<Block> vBlock; // ���⼭ ���� ������ �����ڳ�..?! ���׷��� �޸𸮰� ����ˤ����˰� ������ �䷸�� ���� �׷������
    Block* block;
    Block* block2;
    //Ghost* ghost[20];
    list<Ghost*> vGhost;
    list<GhostCollisionData*> collisionGhost;
    Object* object;
    ObjData* objData[MAX_VAO_TYPE];
    Player* player;
    MapLoader* map;
    Bead* bead;
    PowerBead* powerBead;
    InGameUI* ingameUI;
    Bottom* bottom;
    glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
    glm::vec3 cameraDirection = glm::vec3(0.0f, 0.0f, 0.25f); 
    glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
    glm::vec3 offset = glm::vec3(0.0f, 15.5f, 0.0f); // ī�޶� ����������� ���� ���� �޸�

    float speed = NORMAL_SPEED;
    float degreeCameraRotate = 45.0f; // ���� ��� �ٵ� �� �Ⱦ��� ������?  
    float degreeLightPos = 0.0f;
    float deltaTime = 0.0f;
    float lastFrame = 0.0f;
    float currentFrame = 0.0f;
    float additonalTime = 0.0f;
    float inGameTime = 0.0f;
    float powerBeadTime = 0.0f;
    float angle, lx, lz = 0.0f;
    float ghostSpawnRunningTime = 0.0f;
    glm::vec3 ghostColor = glm::vec3(0.0f, 0.0f, 0.0f);
    int beadNumber = 0;
    bool isDrawFill = true;
    bool isFPS = false;
    bool isGhost = false;
    bool isBead = true;
    bool EatBead = false;
    bool EatPowerBead = false;
    bool CollideBead = false;
    bool isPowerBead = false;
    bool isInitComplete = false;
    bool DeleteHP = false;
    int ghostID;
    Vector3 lightColor = Vector3(1, 1, 1);
    Vector3 lightColor_white = Vector3(1, 1, 1);
    Vector3 lightColor_black = Vector3(0, 0, 0);
    GAMESTATE state;
    GLuint texture[4];
    StartSceneUI* startUI;
    EndingScene* endingUI;
    unsigned int flaglocation;

public:
    static InGameManager& GetInstance() {
        if (instance == NULL) {
            instance = new InGameManager();
        }
        return *instance;
    }

	GLvoid DrawScene(bool isMain);
    GLvoid InitBuffer();
    GLvoid InitShader();
    GLvoid InitObject();

    float GetDegreeCameraRotate() { return this->degreeCameraRotate; }
    float GetDegreeLightPos() { return this->degreeLightPos; }
    float GetDeltaTime() { return this->deltaTime; }
    float GetIngameTime() { return this->inGameTime; }
    float currentTime() { return round(this->inGameTime / 100) / 10 + this->additonalTime; }
    float GetTime();
    void RecordTime();
    string GetBestRecord();
    float GetPlayerHP();
    //int GetBeadNumber() { return this->beadNumber; }
    bool GetPresence() { return this->isBead; }
    void CalculateTime();
    void CameraSetting();
    void TimerFunction();
    void CheckDirection(DynamicObject* dObject);
    Vector3 DirToVec3(DIRECTION dir);
    void CreateGhost(int i, int j, Vector3 position);
    void DeleteGhost(Ghost* g);
    Ghost* FindGhostByID(int id);
    float CountBeadAmount();
    void DecreaseBeadNumber();

    void DrawTextureImage();
    GLuint GetTexture(TextureType type);

    bool GetIsDrawFill() { return this->isDrawFill; }
    glm::vec3 GetCameraPos() { return this->cameraPos; }
    glm::vec3 GetCameraDirection() { return this->cameraDirection; }
    glm::vec3 GetCameraUp() { return this->cameraUp; }
    void SetCameraPos(glm::vec3 camera) { this->cameraPos = camera + offset; }; // set�� void / returnŸ���� �����
    void SetDegreeCameraRotate(float rotate) { this->degreeCameraRotate = rotate; }
    void SetCameraDirection(glm::vec3 dir) {   this->cameraDirection = dir; }
    void SetFPS(bool isOn) { this->isFPS = isOn; }
    Vector3 GetLightColor() { return this->lightColor; }
    void SetLightColor(Vector3 color) { this->lightColor = color; }
    Ghost* GetGhost();
    // float GetHP();

    Player* GetPlayer() { return this->player; }    // GM���� player�� �ҷ��� ����ϰ� �����ϱ� ���⼭ getplayer�� ���� �� �̱��� ���� �ȿ��� player�ҷ��� ����� �� �ְ� ��
    MapLoader* LoadMap() { return this->map; }
    InGameUI* GetInGameUI() { return this->ingameUI; }
    GLvoid DrawMap(){}
    bool GetFPS() { return this->isFPS; }
    void ChangeSpeed(float speed);
    void InitGame();

    GLint GetVAO(ObjectType type) { return this->VAO[type]; }
    ObjData* GetObjData(ObjectType type) { return this->objData[type]; }
    GAMESTATE GetState() { return this->state; }
    int GetBeadCount() { return this->beadNumber; }
    void SetState(GAMESTATE state);
    void InitTexture();


protected:

};