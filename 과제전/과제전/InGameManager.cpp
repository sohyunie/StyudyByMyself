#include "Standard.h"
#include "InGameManager.h"
#include "ReadObj.h"
#include "Ghost.h"
#include "Block.h"
#include "Player.h"
#include "Bead.h"
#include "PowerBead.h"
#include "MapLoader.h"

GLchar* vertexsource, * fragmentsource; // 소스코드 저장 변수
GLuint vertexShader, fragmentShader; // 세이더 객체
InGameManager* InGameManager::instance = nullptr;	//  static은 초기화가 필요 InGameManager*: 변수 타입/ InGameManager::instance : 인게임매니저 안에 private선언되어 있는 instance를 가리킨다. 

char* filetobuf(const char* file)
{
	FILE* fptr;
	long length;
	char* buf;
	fptr = fopen(file, "rb"); // Open file for reading 나빳어ㅐ
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
		cerr << "ERROR: vertex shader 컴파일 실패\n" << errorLog << endl;
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
	glGenVertexArrays(1, &this->VAO[PLAYER]);
	glBindVertexArray(this->VAO[PLAYER]);
	glGenBuffers(3, this->VBO[PLAYER]);

	glBindBuffer(GL_ARRAY_BUFFER, this->VBO[PLAYER][0]);
	glBufferData(GL_ARRAY_BUFFER, this->objData[PLAYER]->vertexCount * sizeof(float) * 3, this->objData[PLAYER]->vPosData, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, NULL);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, this->VBO[PLAYER][1]);
	glBufferData(GL_ARRAY_BUFFER, this->objData[PLAYER]->vertexCount * sizeof(float) * 3, this->objData[PLAYER]->vNormalData, GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, NULL);
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, this->VBO[PLAYER][2]);
	glBufferData(GL_ARRAY_BUFFER, this->objData[PLAYER]->vertexCount * sizeof(float) * 2, this->objData[PLAYER]->vTextureCoordinateData, GL_STATIC_DRAW);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, NULL);
	glEnableVertexAttribArray(2);

	glGenBuffers(1, &this->EBO[PLAYER]);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EBO[PLAYER]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->objData[PLAYER]->indexCount * sizeof(int), this->objData[PLAYER]->indexData, GL_STATIC_DRAW);


	// GHOST
	glGenVertexArrays(1, &this->VAO[GHOST]);
	glBindVertexArray(this->VAO[GHOST]);
	glGenBuffers(3, this->VBO[GHOST]);

	glBindBuffer(GL_ARRAY_BUFFER, this->VBO[GHOST][0]);
	glBufferData(GL_ARRAY_BUFFER, this->objData[GHOST]->vertexCount * sizeof(float) * 3, this->objData[GHOST]->vPosData, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, NULL);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, this->VBO[GHOST][1]);
	glBufferData(GL_ARRAY_BUFFER, this->objData[GHOST]->vertexCount * sizeof(float) * 3, this->objData[GHOST]->vNormalData, GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, NULL);
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, this->VBO[GHOST][2]);
	glBufferData(GL_ARRAY_BUFFER, this->objData[GHOST]->vertexCount * sizeof(float) * 2, this->objData[GHOST]->vTextureCoordinateData, GL_STATIC_DRAW);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, NULL);
	glEnableVertexAttribArray(2);

	glGenBuffers(1, &this->EBO[GHOST]);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EBO[GHOST]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->objData[GHOST]->indexCount * sizeof(int), this->objData[GHOST]->indexData, GL_STATIC_DRAW);


	// BLOCK
	glGenVertexArrays(1, &this->VAO[WALL]);
	glBindVertexArray(this->VAO[WALL]);
	glGenBuffers(3, this->VBO[WALL]);

	glBindBuffer(GL_ARRAY_BUFFER, this->VBO[WALL][0]);
	glBufferData(GL_ARRAY_BUFFER, this->objData[WALL]->vertexCount * sizeof(float) * 3, this->objData[WALL]->vPosData, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, NULL);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, this->VBO[WALL][1]);
	glBufferData(GL_ARRAY_BUFFER, this->objData[WALL]->vertexCount * sizeof(float) * 3, this->objData[WALL]->vNormalData, GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, NULL);
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, this->VBO[WALL][2]);
	glBufferData(GL_ARRAY_BUFFER, this->objData[WALL]->vertexCount * sizeof(float) * 2, this->objData[WALL]->vTextureCoordinateData, GL_STATIC_DRAW);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, NULL);
	glEnableVertexAttribArray(2);

	glGenBuffers(1, &this->EBO[WALL]);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EBO[WALL]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->objData[WALL]->indexCount * sizeof(int), this->objData[WALL]->indexData, GL_STATIC_DRAW);


	// BEAD
	glGenVertexArrays(1, &this->VAO[BEAD]);
	glBindVertexArray(this->VAO[BEAD]);
	glGenBuffers(3, this->VBO[BEAD]);

	glBindBuffer(GL_ARRAY_BUFFER, this->VBO[BEAD][0]);
	glBufferData(GL_ARRAY_BUFFER, this->objData[BEAD]->vertexCount * sizeof(float) * 3, this->objData[BEAD]->vPosData, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, NULL);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, this->VBO[BEAD][1]);
	glBufferData(GL_ARRAY_BUFFER, this->objData[BEAD]->vertexCount * sizeof(float) * 3, this->objData[BEAD]->vNormalData, GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, NULL);
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, this->VBO[BEAD][2]);
	glBufferData(GL_ARRAY_BUFFER, this->objData[BEAD]->vertexCount * sizeof(float) * 2, this->objData[BEAD]->vTextureCoordinateData, GL_STATIC_DRAW);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, NULL);
	glEnableVertexAttribArray(2);

	glGenBuffers(1, &this->EBO[BEAD]);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EBO[BEAD]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->objData[BEAD]->indexCount * sizeof(int), this->objData[BEAD]->indexData, GL_STATIC_DRAW);



	// POWERBEAD
	glGenVertexArrays(1, &this->VAO[POWERBEAD]);
	glBindVertexArray(this->VAO[POWERBEAD]);
	glGenBuffers(3, this->VBO[POWERBEAD]);

	glBindBuffer(GL_ARRAY_BUFFER, this->VBO[POWERBEAD][0]);
	glBufferData(GL_ARRAY_BUFFER, this->objData[POWERBEAD]->vertexCount * sizeof(float) * 3, this->objData[POWERBEAD]->vPosData, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, NULL);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, this->VBO[POWERBEAD][1]);
	glBufferData(GL_ARRAY_BUFFER, this->objData[POWERBEAD]->vertexCount * sizeof(float) * 3, this->objData[POWERBEAD]->vNormalData, GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, NULL);
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, this->VBO[POWERBEAD][2]);
	glBufferData(GL_ARRAY_BUFFER, this->objData[POWERBEAD]->vertexCount * sizeof(float) * 2, this->objData[POWERBEAD]->vTextureCoordinateData, GL_STATIC_DRAW);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, NULL);
	glEnableVertexAttribArray(2);

	glGenBuffers(1, &this->EBO[POWERBEAD]);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EBO[POWERBEAD]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->objData[POWERBEAD]->indexCount * sizeof(int), this->objData[POWERBEAD]->indexData, GL_STATIC_DRAW);
}

GLvoid InGameManager::DrawScene() {
	glUseProgram(s_program);
	
	this->ghost->DrawObject(s_program);
	this->player->DrawObject(s_program);
	this->map->DrawMap(s_program);
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
	//--- 세이더 삭제하기
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
	//--- Shader Program 사용하기
	glUseProgram(s_program);
}

GLvoid InGameManager::InitObject() 
{
	this->block = new Block(Vector3(0, 0, 0));
	this->block2 = new Block(Vector3(10.0, 0, 0));
	this->ghost = new Ghost(Vector3(20.0, 0, 0));
	this->objData[PLAYER] = new ObjData();
	this->objData[GHOST] = new ObjData();
	this->objData[BEAD] = new ObjData();
	this->objData[POWERBEAD] = new ObjData();
	this->objData[WALL] = new ObjData();
	this->player = new Player();
	this->map = new MapLoader(0);
	this->bead = new Bead();
	this->powerBead = new PowerBead();
	ReadObj(FILE_NAME, this->objData[GHOST]->vPosData, this->objData[GHOST]->vNormalData, this->objData[GHOST]->vTextureCoordinateData, this->objData[GHOST]->indexData, this->objData[GHOST]->vertexCount, this->objData[GHOST]->indexCount);
	ReadObj(BEAD_FILE_NAME, this->objData[BEAD]->vPosData, this->objData[BEAD]->vNormalData, this->objData[BEAD]->vTextureCoordinateData, this->objData[BEAD]->indexData, this->objData[BEAD]->vertexCount, this->objData[BEAD]->indexCount);
	ReadObj(POWERBEAD_FILE_NAME, this->objData[POWERBEAD]->vPosData, this->objData[POWERBEAD]->vNormalData, this->objData[POWERBEAD]->vTextureCoordinateData, this->objData[POWERBEAD]->indexData, this->objData[POWERBEAD]->vertexCount, this->objData[POWERBEAD]->indexCount);
	ReadObj(CUBE_FILE_NAME, this->objData[WALL]->vPosData, this->objData[WALL]->vNormalData, this->objData[WALL]->vTextureCoordinateData, this->objData[WALL]->indexData, this->objData[WALL]->vertexCount, this->objData[WALL]->indexCount);
	ReadObj(CUBE_FILE_NAME, this->objData[PLAYER]->vPosData, this->objData[PLAYER]->vNormalData, this->objData[PLAYER]->vTextureCoordinateData, this->objData[PLAYER]->indexData, this->objData[PLAYER]->vertexCount, this->objData[PLAYER]->indexCount);

	cout << "test" << endl;
	// vBlock.push_back(Block());
}