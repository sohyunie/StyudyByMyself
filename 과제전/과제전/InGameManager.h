#pragma once
class Block;    // ����� ������ Block�̶�� �ְ� �����ظ� �� �� ����
class Ghost;
class Player;
class MapLoader;
class Bead;
class PowerBead;

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
    Ghost* ghost;
    ObjData* objData[MAX_VAO_TYPE];
    Player* player;
    MapLoader* map;
    Bead* bead;
    PowerBead* powerBead;
    glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
    glm::vec3 cameraDirection = glm::vec3(0.0f, 0.0f, -1.0f);
    glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
    glm::vec3 offset = glm::vec3(0.0f, 15.5f, 0.0f); // ī�޶� ����������� ���� ���� �޸�
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
    void SetCameraPos(glm::vec3 camera) { this->cameraPos = camera + offset; }; // set�� void / returnŸ���� �����
    void SetDegreeCameraRotate(float rotate) { this->degreeCameraRotate = rotate; }
    void SetCameraDirection(glm::vec3 dir) {   this->cameraDirection = dir; }
    
    Player* GetPlayer() { return this->player; }    // GM���� player�� �ҷ��� ����ϰ� �����ϱ� ���⼭ getplayer�� ���� �� �̱��� ���� �ȿ��� player�ҷ��� ����� �� �ְ� ��
    MapLoader* LoadMap() { return this->map; }
    GLvoid DrawMap(){}

    GLint GetVAO(ObjectType type) { return this->VAO[type]; }
    ObjData* GetObjData(ObjectType type) { return this->objData[type]; }
protected:

};