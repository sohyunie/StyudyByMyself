#include "Standard.h"
#include "InGameManager.h"
#include "ReadObj.h"
#include "Ghost.h"
#include "Block.h"
#include "Player.h"
#include "Bead.h"
#include "PowerBead.h"
#include "MapLoader.h"

GLchar* vertexsource, * fragmentsource; // �ҽ��ڵ� ���� ����
GLuint vertexShader, fragmentShader; // ���̴� ��ü
InGameManager* InGameManager::instance = nullptr;	//  static�� �ʱ�ȭ�� �ʿ� InGameManager*: ���� Ÿ��/ InGameManager::instance : �ΰ��ӸŴ��� �ȿ� private����Ǿ� �ִ� instance�� ����Ų��. 

char* filetobuf(const char* file)
{
	FILE* fptr;
	long length;
	char* buf;
	fptr = fopen(file, "rb"); // Open file for reading �������
	if (!fptr) // Return NULL on failure
		return NULL;
	fseek(fptr, 0, SEEK_END); // Seek to the end of the file
	length = ftell(fptr); // Find out how many bytes into the file we are
	buf = (char*)malloc(length + 1); // Allocate a buffer for the entire length of the file and a null terminator
	fseek(fptr, 0, SEEK_SET); // Go back to the beginning of the file
	fread(buf, length, 1, fptr); // Read the contents of the file in to the buffer
	fclose(fptr); // Close the file
	buf[length] = 0; // Null terminator
	return buf; // Return the buffer
}

void make_vertexShaders()
{
	char* vertexsource = filetobuf("vertex.glsl");

	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, (const GLchar**)&vertexsource, 0);
	glCompileShader(vertexShader);

	GLint result;
	GLchar errorLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &result);
	if (!result)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, errorLog);
		cerr << "ERROR: vertex shader ������ ����\n" << errorLog << endl;
	}
}


void make_fragmentShaders()
{
	char* fragmentsource = filetobuf("fragment.glsl");

	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, (const GLchar**)&fragmentsource, 0);
	glCompileShader(fragmentShader);

	GLint result;
	GLchar errorLog[512];
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &result);
	if (!result)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, errorLog);
		cerr << "ERROR: fragment shader error\n" << errorLog << endl;
	}
}


GLuint s_program;

GLvoid InGameManager::InitBuffer() {
	cout << "InitBuffer InGameManager" << endl;
	// PLAYER
	glGenVertexArrays(1, &this->VAO[PLAYER]); //--- VAO �� �����ϰ� �Ҵ��ϱ�
	glBindVertexArray(this->VAO[PLAYER]); //--- VAO�� ���ε��ϱ�
	glGenBuffers(2, &this->VBO[PLAYER][0]); //--- 2���� VBO�� �����ϰ� �Ҵ��ϱ�
	//--- 1��° VBO�� Ȱ��ȭ�Ͽ� ���ε��ϰ�, ���ؽ� �Ӽ� (��ǥ��)�� ����
	glBindBuffer(GL_ARRAY_BUFFER, this->VBO[PLAYER][0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(blockVertex), blockVertex, GL_STATIC_DRAW);
	// ��ǥ���� attribute �ε��� 0���� ����Ѵ�: ���ؽ� �� 3* float
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	// attribute �ε��� 0���� ��밡���ϰ� ��
	glEnableVertexAttribArray(0);
	// ������ attribute �ε��� 1���� ����Ѵ�
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	// attribute �ε��� 1���� ��� �����ϰ� ��.
	glEnableVertexAttribArray(1);


	// GHOST
	glGenVertexArrays(1, &this->VAO[GHOST]);
	glBindVertexArray(this->VAO[GHOST]);
	glGenBuffers(3, this->VBO[GHOST]);

	glBindBuffer(GL_ARRAY_BUFFER, this->VBO[GHOST][0]);
	glBufferData(GL_ARRAY_BUFFER, this->gobj[GHOST]->vertexCount * sizeof(float) * 3, this->gobj[GHOST]->vPosData, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, NULL);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, this->VBO[GHOST][1]);
	glBufferData(GL_ARRAY_BUFFER, this->gobj[GHOST]->vertexCount * sizeof(float) * 3, this->gobj[GHOST]->vNormalData, GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, NULL);
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, this->VBO[GHOST][2]);
	glBufferData(GL_ARRAY_BUFFER, this->gobj[GHOST]->vertexCount * sizeof(float) * 2, this->gobj[GHOST]->vTextureCoordinateData, GL_STATIC_DRAW);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, NULL);
	glEnableVertexAttribArray(2);

	glGenBuffers(1, &this->EBO[GHOST]);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EBO[GHOST]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->gobj[GHOST]->indexCount * sizeof(int), this->gobj[GHOST]->indexData, GL_STATIC_DRAW);


	// BLOCK
	glGenVertexArrays(1, &this->VAO[BLOCK]); //--- VAO �� �����ϰ� �Ҵ��ϱ�
	glBindVertexArray(this->VAO[BLOCK]); //--- VAO�� ���ε��ϱ�
	glGenBuffers(2, &this->VBO[BLOCK][0]); //--- 2���� VBO�� �����ϰ� �Ҵ��ϱ�
	//--- 1��° VBO�� Ȱ��ȭ�Ͽ� ���ε��ϰ�, ���ؽ� �Ӽ� (��ǥ��)�� ����
	glBindBuffer(GL_ARRAY_BUFFER, this->VBO[BLOCK][0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(blockVertex), blockVertex, GL_STATIC_DRAW);
	// ��ǥ���� attribute �ε��� 0���� ����Ѵ�: ���ؽ� �� 3* float
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	// attribute �ε��� 0���� ��밡���ϰ� ��
	glEnableVertexAttribArray(0);
	// ������ attribute �ε��� 1���� ����Ѵ�
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	// attribute �ε��� 1���� ��� �����ϰ� ��.
	glEnableVertexAttribArray(1);


	// BEAD
	glGenVertexArrays(1, &this->VAO[BEAD]);
	glBindVertexArray(this->VAO[BEAD]);
	glGenBuffers(3, this->VBO[BEAD]);

	glBindBuffer(GL_ARRAY_BUFFER, this->VBO[BEAD][0]);
	glBufferData(GL_ARRAY_BUFFER, this->gobj[BEAD]->vertexCount * sizeof(float) * 3, this->gobj[BEAD]->vPosData, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, NULL);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, this->VBO[BEAD][1]);
	glBufferData(GL_ARRAY_BUFFER, this->gobj[BEAD]->vertexCount * sizeof(float) * 3, this->gobj[BEAD]->vNormalData, GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, NULL);
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, this->VBO[BEAD][2]);
	glBufferData(GL_ARRAY_BUFFER, this->gobj[BEAD]->vertexCount * sizeof(float) * 2, this->gobj[BEAD]->vTextureCoordinateData, GL_STATIC_DRAW);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, NULL);
	glEnableVertexAttribArray(2);

	glGenBuffers(1, &this->EBO[BEAD]);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EBO[BEAD]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->gobj[BEAD]->indexCount * sizeof(int), this->gobj[BEAD]->indexData, GL_STATIC_DRAW);



	// POWERBEAD
	glGenVertexArrays(1, &this->VAO[POWERBEAD]);
	glBindVertexArray(this->VAO[POWERBEAD]);
	glGenBuffers(3, this->VBO[POWERBEAD]);

	glBindBuffer(GL_ARRAY_BUFFER, this->VBO[POWERBEAD][0]);
	glBufferData(GL_ARRAY_BUFFER, this->gobj[POWERBEAD]->vertexCount * sizeof(float) * 3, this->gobj[POWERBEAD]->vPosData, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, NULL);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, this->VBO[POWERBEAD][1]);
	glBufferData(GL_ARRAY_BUFFER, this->gobj[POWERBEAD]->vertexCount * sizeof(float) * 3, this->gobj[POWERBEAD]->vNormalData, GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, NULL);
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, this->VBO[POWERBEAD][2]);
	glBufferData(GL_ARRAY_BUFFER, this->gobj[POWERBEAD]->vertexCount * sizeof(float) * 2, this->gobj[POWERBEAD]->vTextureCoordinateData, GL_STATIC_DRAW);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, NULL);
	glEnableVertexAttribArray(2);

	glGenBuffers(1, &this->EBO[POWERBEAD]);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EBO[POWERBEAD]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->gobj[POWERBEAD]->indexCount * sizeof(int), this->gobj[POWERBEAD]->indexData, GL_STATIC_DRAW);
}

GLvoid InGameManager::DrawScene() {
	glUseProgram(s_program);
	
	this->ghost->DrawObject(s_program, this->VAO[GHOST], this->gobj[GHOST]->indexCount);
	this->player->DrawObject(s_program, this->VAO[PLAYER], 36);
	//this->map->DrawMap(s_program, this->VAO, 36);
	//this->bead->DrawObject(s_program, this->VAO[BEAD], this->gobj[BEAD]->indexCount);
	//this->powerBead->DrawObject(s_program, this->VAO[POWERBEAD], this->gobj[POWERBEAD]->indexCount);
}

GLvoid InGameManager::InitShader() {
	cout << "InitShader GameManager" << endl;
	make_vertexShaders();
	make_fragmentShaders();
	//-- shader Program
	s_program = glCreateProgram();
	glAttachShader(s_program, vertexShader);
	glAttachShader(s_program, fragmentShader);
	glLinkProgram(s_program);
	//--- ���̴� �����ϱ�
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
	//--- Shader Program ����ϱ�
	glUseProgram(s_program);
}

GLvoid InGameManager::InitObject() 
{
	this->block = new Block(Vector3(0, 0, 0));
	this->block2 = new Block(Vector3(10.0, 0, 0));
	this->ghost = new Ghost(Vector3(20.0, 0, 0));
	this->gobj[GHOST] = new ObjData();
	this->gobj[BEAD] = new ObjData();
	this->gobj[POWERBEAD] = new ObjData();
	this->player = new Player();
	this->map = new MapLoader(0);
	this->bead = new Bead(Vector3(-10.0,0.0,0.0));
	this->powerBead = new PowerBead(Vector3(-20.0,0.0,0.0));
	ReadObj(FILE_NAME, this->gobj[GHOST]->vPosData, this->gobj[GHOST]->vNormalData, this->gobj[GHOST]->vTextureCoordinateData, this->gobj[GHOST]->indexData, this->gobj[GHOST]->vertexCount, this->gobj[GHOST]->indexCount);
	ReadObj(BEAD_FILE_NAME, this->gobj[BEAD]->vPosData, this->gobj[BEAD]->vNormalData, this->gobj[BEAD]->vTextureCoordinateData, this->gobj[BEAD]->indexData, this->gobj[BEAD]->vertexCount, this->gobj[BEAD]->indexCount);
	ReadObj(POWERBEAD_FILE_NAME, this->gobj[POWERBEAD]->vPosData, this->gobj[POWERBEAD]->vNormalData, this->gobj[POWERBEAD]->vTextureCoordinateData, this->gobj[POWERBEAD]->indexData, this->gobj[POWERBEAD]->vertexCount, this->gobj[POWERBEAD]->indexCount);

	cout << "test" << endl;
	// vBlock.push_back(Block());
}