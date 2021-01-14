#include "InGameManager.h"

InGameManager* InGameManager::instance = NULL;

GLchar* vertexsource, * fragmentsource; // 소스코드 저장 변수
GLuint vertexshader, fragmentshader; // 세이더 객체

GLuint VAO, VBO;

char* filetobuf(const char* file)
{
	FILE* fptr;
	long length;
	char* buf;
	fptr = fopen(file, "rb"); // Open file for reading
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
void make_vertexShader()
{
	vertexsource = filetobuf("vertex.glsl");
	//--- 버텍스 세이더 객체 만들기
	vertexshader = glCreateShader(GL_VERTEX_SHADER);
	//--- 세이더 코드를 세이더 객체에 넣기
	glShaderSource(vertexshader, 1, (const GLchar**)&vertexsource, 0);
	//--- 버텍스 세이더 컴파일하기
	glCompileShader(vertexshader);
	//--- 컴파일이 제대로 되지 않은 경우: 에러 체크
	GLint result;
	GLchar errorLog[512];
	glGetShaderiv(vertexshader, GL_COMPILE_STATUS, &result);
	if (!result)
	{
		glGetShaderInfoLog(vertexshader, 512, NULL, errorLog);
		cerr << "ERROR: vertex shader 컴파일 실패\n" << errorLog << endl;
		return;
	}
}

void make_fragmentShader()
{
	fragmentsource = filetobuf("fragment.glsl");
	//--- 프래그먼트 세이더 객체 만들기
	fragmentshader = glCreateShader(GL_FRAGMENT_SHADER);
	//--- 세이더 코드를 세이더 객체에 넣기
	glShaderSource(fragmentshader, 1, (const GLchar**)&fragmentsource, 0);
	//--- 프래그먼트 세이더 컴파일
	glCompileShader(fragmentshader);
	//--- 컴파일이 제대로 되지 않은 경우: 컴파일 에러 체크
	GLint result;
	GLchar errorLog[512];
	glGetShaderiv(fragmentshader, GL_COMPILE_STATUS, &result);
	if (!result)
	{
		glGetShaderInfoLog(fragmentshader, 512, NULL, errorLog);
		cerr << "ERROR: fragment shader 컴파일 실패\n" << errorLog << endl;
		return;
	}
}

GLuint s_program;
//--- 세이더 프로그램 생성하기
void InitShader()
{
	cout << "InitShader GameManager" << endl;
	make_vertexShader();
	make_fragmentShader();
	//-- shader Program
	s_program = glCreateProgram();
	glAttachShader(s_program, vertexshader);
	glAttachShader(s_program, fragmentshader);
	glLinkProgram(s_program);
	//--- 세이더 삭제하기
	glDeleteShader(vertexshader);
	glDeleteShader(fragmentshader);
	//--- Shader Program 사용하기
	glUseProgram(s_program);
}

GLvoid InGameManager::DrawScene() {
	glUseProgram(s_program);
	cout << "DrawScene InGameManager" << endl;
	glm::mat4 TR = glm::mat4(1.0f); //--- transformation matrix
	glm::mat4 Rz = glm::mat4(1.0f); //--- rotation matrix
	glm::mat4 Tx = glm::mat4(1.0f); //--- transformation matrix
	Tx = glm::translate(Tx, glm::vec3(0.5, 0.0, 0.0)); //--- x축으로 translation
	Rz = glm::rotate(Rz, glm::radians(45.0f), glm::vec3(0.0f, 1.0f, 0.0f)); //--- z축에대하여 회전
	TR = Rz * Tx; //--- 합성 변환 행렬: translate -> rotate

	glm::vec3 cameraPos = glm::vec3(0.0f, 2.0f, 5.0f);
	glm::vec3 cameraDirection = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
	glm::mat4 view = glm::mat4(1.0f);

	glm::mat4 projection = glm::mat4(1.0f);
	projection = glm::perspective(glm::radians(45.0f), (float)1200 / (float)800, 0.1f, 100.0f);

	unsigned int modelLocation = glGetUniformLocation(s_program, "modelTransform"); //--- 버텍스 세이더에서모델 변환 위치 가져오기
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(TR)); //--- modelTransform 변수에 변환 값 적용하기

	view = glm::lookAt(cameraPos, cameraDirection, cameraUp);
	unsigned int viewLocation = glGetUniformLocation(s_program, "viewTransform");	// 버텍스 사이에서 viewTransform 변수위치
	glUniformMatrix4fv(viewLocation, 1, GL_FALSE, &view[0][0]);	// viewTransform 변수에 변환값 적용하기

	unsigned int projectionLocation = glGetUniformLocation(s_program, "projectionTransform");
	glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, &projection[0][0]);

	int lightPosLocation = glGetUniformLocation(s_program, "lightPos"); //--- lightPos 값 전달: (0.0, 0.0, 5.0);
	glUniform3f(lightPosLocation, 0.0, 0.0, 5.0);

	int lightColorLocation = glGetUniformLocation(s_program, "lightColor"); //--- lightColor 값 전달: (1.0, 1.0, 1.0) 백색
	glUniform3f(lightColorLocation, 1.0, 1.0, 1.0);

	int objColorLocation = glGetUniformLocation(s_program, "objectColor"); //--- object Color값 전달: (1.0, 0.5, 0.3)의 색
	glUniform3f(objColorLocation, 1.0, 1.0, 1.0);

	int ViewLocation = glGetUniformLocation(s_program, "viewPos");
	glUniform3f(ViewLocation, cameraPos.x, cameraPos.y, cameraPos.z);

	// 사용할 VAO 불러오기
	glBindVertexArray(VAO);
	// 삼각형 그리기
	glDrawArrays(GL_TRIANGLES, 0, 36);
}

GLvoid InGameManager::InitBuffer() {
	cout << "InitBuffer InGameManager" << endl;
	// PLAYER

	// GHOST

	// BLOCK
	glGenVertexArrays(1, &VAO); //--- VAO 를 지정하고 할당하기
	glBindVertexArray(VAO); //--- VAO를 바인드하기
	glGenBuffers(2, &VBO); //--- 2개의 VBO를 지정하고 할당하기
	//--- 1번째 VBO를 활성화하여 바인드하고, 버텍스 속성 (좌표값)을 저장
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(blockVertex), blockVertex, GL_STATIC_DRAW);
	// 좌표값을 attribute 인덱스 0번에 명시한다: 버텍스 당 3* float
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	// attribute 인덱스 0번을 사용가능하게 함
	glEnableVertexAttribArray(0);
	// 색상값을 attribute 인덱스 1번에 명시한다: 버텍스 당 3*float
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	// attribute 인덱스 1번을 사용 가능하게 함.
	glEnableVertexAttribArray(1);
}