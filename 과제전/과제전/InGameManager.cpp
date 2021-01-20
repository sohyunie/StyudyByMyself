#include "standard.h"
#include "InGameManager.h"
#include "ReadObj.h"
#include "Ghost.h"
#include "Block.h"

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

	// GHOST
	glGenVertexArrays(1, &this->VAO[GHOST]);
	glBindVertexArray(this->VAO[GHOST]);
	glGenBuffers(3, this->VBO[GHOST]);

	glBindBuffer(GL_ARRAY_BUFFER, this->VBO[GHOST][0]);
	glBufferData(GL_ARRAY_BUFFER, this->gobj->vertexCount * sizeof(float) * 3, this->gobj->vPosData, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, NULL);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, this->VBO[GHOST][1]);
	glBufferData(GL_ARRAY_BUFFER, this->gobj->vertexCount * sizeof(float) * 3, this->gobj->vNormalData, GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, NULL);
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, this->VBO[GHOST][2]);
	glBufferData(GL_ARRAY_BUFFER, this->gobj->vertexCount * sizeof(float) * 2, this->gobj->vTextureCoordinateData, GL_STATIC_DRAW);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, NULL);
	glEnableVertexAttribArray(2);

	glGenBuffers(1, &this->EBO[GHOST]);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EBO[GHOST]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->gobj->indexCount * sizeof(int), this->gobj->indexData, GL_STATIC_DRAW);


	// BLOCK
	//glGenVertexArrays(1, &this->VAO[BLOCK]); //--- VAO 를 지정하고 할당하기
	//glBindVertexArray(this->VAO[BLOCK]); //--- VAO를 바인드하기
	//glGenBuffers(2, &this->VBO[BLOCK][0]); //--- 2개의 VBO를 지정하고 할당하기
	////--- 1번째 VBO를 활성화하여 바인드하고, 버텍스 속성 (좌표값)을 저장
	//glBindBuffer(GL_ARRAY_BUFFER, this->VBO[BLOCK][0]);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(blockVertex), blockVertex, GL_STATIC_DRAW);
	//// 좌표값을 attribute 인덱스 0번에 명시한다: 버텍스 당 3* float
	//glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	//// attribute 인덱스 0번을 사용가능하게 함
	//glEnableVertexAttribArray(0);
	//// 색상값을 attribute 인덱스 1번에 명시한다
	//glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	//// attribute 인덱스 1번을 사용 가능하게 함.
	//glEnableVertexAttribArray(1);
}

GLvoid InGameManager::DrawScene() {
	//glUseProgram(s_program);

	//glm::mat4 modelTransform = glm::mat4(1.0f);
	//modelTransform = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0));

	//glm::vec4 cameraPos = glm::rotate(glm::mat4(1.0f), glm::radians(InGameManager::GetInstance().degreeCameraRotate), glm::vec3(0.0f, 1.0f, 0.0f)) * glm::vec4(CAMERA_POS, 1.0f);
	//glm::mat4 view = glm::lookAt(glm::vec3(cameraPos), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	//glm::mat4 proj = glm::perspective(glm::radians(60.0f), WINDOW_WITDH / (float)WINDOW_HEIGHT, 0.001f, 1000.f);

	//GLuint modelTransformLocation = glGetUniformLocation(s_program, "g_modelTransform");
	//glUniformMatrix4fv(modelTransformLocation, 1, GL_FALSE, glm::value_ptr(modelTransform));

	//GLuint viewLocation = glGetUniformLocation(s_program, "g_view");
	//glUniformMatrix4fv(viewLocation, 1, GL_FALSE, glm::value_ptr(view));

	//GLuint projectLocation = glGetUniformLocation(s_program, "g_projection");
	//glUniformMatrix4fv(projectLocation, 1, GL_FALSE, glm::value_ptr(proj));

	//glm::vec3 lightAmbient = LIGHT_AMBIENT;
	//GLuint lightAmbientLocation = glGetUniformLocation(s_program, "g_lightAmbient");
	//glUniform3fv(lightAmbientLocation, 1, (float*)&lightAmbient);

	//glm::vec3 lightPos = LIGHT_POS;
	//GLuint lightPosLocation = glGetUniformLocation(s_program, "g_lightPos");
	//glUniform3fv(lightPosLocation, 1, (float*)&lightPos);

	//glm::vec3 lightColor = LIGHT_COLOR;
	//GLuint lightColorLocation = glGetUniformLocation(s_program, "g_lightColor");
	//glUniform3fv(lightColorLocation, 1, (float*)&lightColor);

	//glm::vec3 objColor = OBJECT_COLOR;
	//GLuint objColorLocation = glGetUniformLocation(s_program, "g_objectColor");
	//glUniform3fv(objColorLocation, 1, (float*)&objColor);

	//GLuint cameraPosLocation = glGetUniformLocation(s_program, "g_cameraPos");
	//glUniform3fv(cameraPosLocation, 1, (float*)&cameraPos);

	//glBindVertexArray(InGameManager::GetInstance().VAO[4]);
	//glDrawElements(GL_TRIANGLES, InGameManager::GetInstance().gobj->indexCount, GL_UNSIGNED_INT, 0);
	glUseProgram(s_program);
	this->block->DrawObject(s_program, this->VAO[BLOCK], 36);
	this->ghost->DrawObject(s_program, this->VAO[GHOST], this->gobj->indexCount);
	this->block2->DrawObject(s_program, this->VAO[BLOCK], 36);
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
	this->block2 = new Block(Vector3(10, 0, 0));
	this->ghost = new Ghost();
	this->gobj = new ObjData();

	ReadObj(FILE_NAME, this->gobj->vPosData, this->gobj->vNormalData, this->gobj->vTextureCoordinateData, this->gobj->indexData, this->gobj->vertexCount, this->gobj->indexCount);

	cout << "test" << endl;
	// vBlock.push_back(Block());
}