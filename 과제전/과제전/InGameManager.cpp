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
	glGenVertexArrays(1, &this->VAO[PLAYER]); //--- VAO 를 지정하고 할당하기
	glBindVertexArray(this->VAO[PLAYER]); //--- VAO를 바인드하기
	glGenBuffers(2, &this->VBO[PLAYER][0]); //--- 2개의 VBO를 지정하고 할당하기
	//--- 1번째 VBO를 활성화하여 바인드하고, 버텍스 속성 (좌표값)을 저장
	glBindBuffer(GL_ARRAY_BUFFER, this->VBO[PLAYER][0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(blockVertex), blockVertex, GL_STATIC_DRAW);
	// 좌표값을 attribute 인덱스 0번에 명시한다: 버텍스 당 3* float
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	// attribute 인덱스 0번을 사용가능하게 함
	glEnableVertexAttribArray(0);
	// 색상값을 attribute 인덱스 1번에 명시한다
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	// attribute 인덱스 1번을 사용 가능하게 함.
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
	glGenVertexArrays(1, &this->VAO[BLOCK]); //--- VAO 를 지정하고 할당하기
	glBindVertexArray(this->VAO[BLOCK]); //--- VAO를 바인드하기
	glGenBuffers(2, &this->VBO[BLOCK][0]); //--- 2개의 VBO를 지정하고 할당하기
	//--- 1번째 VBO를 활성화하여 바인드하고, 버텍스 속성 (좌표값)을 저장
	glBindBuffer(GL_ARRAY_BUFFER, this->VBO[BLOCK][0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(blockVertex), blockVertex, GL_STATIC_DRAW);
	// 좌표값을 attribute 인덱스 0번에 명시한다: 버텍스 당 3* float
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	// attribute 인덱스 0번을 사용가능하게 함
	glEnableVertexAttribArray(0);
	// 색상값을 attribute 인덱스 1번에 명시한다
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	// attribute 인덱스 1번을 사용 가능하게 함.
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