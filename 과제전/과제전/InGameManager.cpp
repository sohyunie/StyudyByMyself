#include "ReadObj.h"

#include <iostream>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <random>
#include <time.h>
#include <windows.h>
#include <vector>
#include <gl/glew.h> 
#include <gl/freeglut.h>
#include <gl/freeglut_ext.h>
#include <gl/glm/glm.hpp>
#include <gl/glm/ext.hpp>
#include <gl/glm/gtc/matrix_transform.hpp>
#include <gl/glm/gtc/type_ptr.hpp>
using namespace std;
#pragma warning(disable:4996)

const GLfloat triShape[3][3] = { // 삼각형 위치 값
	{ -0.5, -0.5, 0.0 },	
	{ 0.5, -0.5, 0.0 },		
	{ 0.0, 0.5, 0.0} 
};
const GLfloat colors[3][3] = { // 삼각형 꼭짓점 색상
	{ 1.0, 0.0, 0.0 },	
	{ 0.0, 1.0, 0.0 },	
	{ 0.0, 0.0, 1.0 } 
};

GLuint VAO, VBO[2], VBOPosition, VBOColor;

GLchar* vertexsource, * fragmentsource; // 소스코드 저장 변수
GLuint vertexshader, fragmentshader; // 세이더 객체


GLvoid Reshape(int w, int h) {
	glViewport(0, 0, w, h);
}

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
	if(!result)
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

//--- 버퍼 생성하고 데이터 받아오기
GLvoid InitBuffer()
{
	glGenVertexArrays(1, &VAO); //--- VAO 를 지정하고 할당하기
	glBindVertexArray(VAO); //--- VAO를 바인드하기
	glGenBuffers(2, &VBO[2]); //--- 2개의 VBO를 지정하고 할당하기
	//--- 1번째 VBO를 활성화하여 바인드하고, 버텍스 속성 (좌표값)을 저장
	glBindBuffer(GL_ARRAY_BUFFER, VBOPosition);
	// 변수 triShape 에서 버텍스 데이터 값을 버퍼에 복사한다.
	// triShape 배열의 사이즈: 9 * float
	glBufferData(GL_ARRAY_BUFFER, 9 * sizeof(GLfloat), triShape, GL_STATIC_DRAW);
	// 좌표값을 attribute 인덱스 0번에 명시한다: 버텍스 당 3* float
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	// attribute 인덱스 0번을 사용가능하게 함
	glEnableVertexAttribArray(0);
	//--- 2번째 VBO를 활성화 하여 바인드 하고, 버텍스 속성 (색상)을 저장
	glBindBuffer(GL_ARRAY_BUFFER, VBOColor);
	// 변수 colors에서 버텍스 색상을 복사한다.
	// colors 배열의 사이즈: 9 *float
	glBufferData(GL_ARRAY_BUFFER, 9 * sizeof(GLfloat), colors, GL_STATIC_DRAW);
	// 색상값을 attribute 인덱스 1번에 명시한다: 버텍스 당 3*float
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	// attribute 인덱스 1번을 사용 가능하게 함.
	glEnableVertexAttribArray(1);
}

//--- 세이더 프로그램 생성하기
GLuint s_program;
void InitShader()
{
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

GLvoid drawScene() //--- 콜백 함수: 그리기 콜백 함수
{
	//--- 변경된 배경색 설정
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	// 렌더링 파이프라인에 세이더 불러오기
	glUseProgram(s_program);
	// 사용할 VAO 불러오기
	glBindVertexArray(VAO);
	// 삼각형 그리기
	glDrawArrays(GL_TRIANGLES, 0, 3);
	glutSwapBuffers(); // 화면에 출력하기
}

//--- 메인 함수

void main(int argc, char** argv) //--- 윈도우 출력하고 콜백함수 설정
{
	//--- 윈도우 생성하기
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(800, 600);
	glutCreateWindow("Example1");
	//--- GLEW 초기화하기
	glewExperimental = GL_TRUE;
	glewInit();
	InitShader();
	InitBuffer();
	glutDisplayFunc(drawScene);
	glutReshapeFunc(Reshape);
	glutMainLoop();
}