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

const GLfloat triShape[3][3] = { // �ﰢ�� ��ġ ��
	{ -0.5, -0.5, 0.0 },	
	{ 0.5, -0.5, 0.0 },		
	{ 0.0, 0.5, 0.0} 
};
const GLfloat colors[3][3] = { // �ﰢ�� ������ ����
	{ 1.0, 0.0, 0.0 },	
	{ 0.0, 1.0, 0.0 },	
	{ 0.0, 0.0, 1.0 } 
};

GLuint VAO, VBO[2], VBOPosition, VBOColor;

GLchar* vertexsource, * fragmentsource; // �ҽ��ڵ� ���� ����
GLuint vertexshader, fragmentshader; // ���̴� ��ü


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
		//--- ���ؽ� ���̴� ��ü �����
	vertexshader = glCreateShader(GL_VERTEX_SHADER);
	//--- ���̴� �ڵ带 ���̴� ��ü�� �ֱ�
	glShaderSource(vertexshader, 1, (const GLchar**)&vertexsource, 0);
	//--- ���ؽ� ���̴� �������ϱ�
	glCompileShader(vertexshader);
	//--- �������� ����� ���� ���� ���: ���� üũ
	GLint result;
	GLchar errorLog[512];
	glGetShaderiv(vertexshader, GL_COMPILE_STATUS, &result);
	if(!result)
	{
		glGetShaderInfoLog(vertexshader, 512, NULL, errorLog);
		cerr << "ERROR: vertex shader ������ ����\n" << errorLog << endl;
		return;
	}
}

void make_fragmentShader()
{
	fragmentsource = filetobuf("fragment.glsl");
	//--- �����׸�Ʈ ���̴� ��ü �����
	fragmentshader = glCreateShader(GL_FRAGMENT_SHADER);
	//--- ���̴� �ڵ带 ���̴� ��ü�� �ֱ�
	glShaderSource(fragmentshader, 1, (const GLchar**)&fragmentsource, 0);
	//--- �����׸�Ʈ ���̴� ������
	glCompileShader(fragmentshader);
	//--- �������� ����� ���� ���� ���: ������ ���� üũ
	GLint result;
	GLchar errorLog[512];
	glGetShaderiv(fragmentshader, GL_COMPILE_STATUS, &result);
	if (!result)
	{
		glGetShaderInfoLog(fragmentshader, 512, NULL, errorLog);
		cerr << "ERROR: fragment shader ������ ����\n" << errorLog << endl;
		return;
	}
}

//--- ���� �����ϰ� ������ �޾ƿ���
GLvoid InitBuffer()
{
	glGenVertexArrays(1, &VAO); //--- VAO �� �����ϰ� �Ҵ��ϱ�
	glBindVertexArray(VAO); //--- VAO�� ���ε��ϱ�
	glGenBuffers(2, &VBO[2]); //--- 2���� VBO�� �����ϰ� �Ҵ��ϱ�
	//--- 1��° VBO�� Ȱ��ȭ�Ͽ� ���ε��ϰ�, ���ؽ� �Ӽ� (��ǥ��)�� ����
	glBindBuffer(GL_ARRAY_BUFFER, VBOPosition);
	// ���� triShape ���� ���ؽ� ������ ���� ���ۿ� �����Ѵ�.
	// triShape �迭�� ������: 9 * float
	glBufferData(GL_ARRAY_BUFFER, 9 * sizeof(GLfloat), triShape, GL_STATIC_DRAW);
	// ��ǥ���� attribute �ε��� 0���� ����Ѵ�: ���ؽ� �� 3* float
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	// attribute �ε��� 0���� ��밡���ϰ� ��
	glEnableVertexAttribArray(0);
	//--- 2��° VBO�� Ȱ��ȭ �Ͽ� ���ε� �ϰ�, ���ؽ� �Ӽ� (����)�� ����
	glBindBuffer(GL_ARRAY_BUFFER, VBOColor);
	// ���� colors���� ���ؽ� ������ �����Ѵ�.
	// colors �迭�� ������: 9 *float
	glBufferData(GL_ARRAY_BUFFER, 9 * sizeof(GLfloat), colors, GL_STATIC_DRAW);
	// ������ attribute �ε��� 1���� ����Ѵ�: ���ؽ� �� 3*float
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	// attribute �ε��� 1���� ��� �����ϰ� ��.
	glEnableVertexAttribArray(1);
}

//--- ���̴� ���α׷� �����ϱ�
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
	//--- ���̴� �����ϱ�
	glDeleteShader(vertexshader);
	glDeleteShader(fragmentshader);
	//--- Shader Program ����ϱ�
	glUseProgram(s_program);
}

GLvoid drawScene() //--- �ݹ� �Լ�: �׸��� �ݹ� �Լ�
{
	//--- ����� ���� ����
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	// ������ ���������ο� ���̴� �ҷ�����
	glUseProgram(s_program);
	// ����� VAO �ҷ�����
	glBindVertexArray(VAO);
	// �ﰢ�� �׸���
	glDrawArrays(GL_TRIANGLES, 0, 3);
	glutSwapBuffers(); // ȭ�鿡 ����ϱ�
}

//--- ���� �Լ�

void main(int argc, char** argv) //--- ������ ����ϰ� �ݹ��Լ� ����
{
	//--- ������ �����ϱ�
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(800, 600);
	glutCreateWindow("Example1");
	//--- GLEW �ʱ�ȭ�ϱ�
	glewExperimental = GL_TRUE;
	glewInit();
	InitShader();
	InitBuffer();
	glutDisplayFunc(drawScene);
	glutReshapeFunc(Reshape);
	glutMainLoop();
}