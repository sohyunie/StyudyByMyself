#include "Standard.h"
#include "InGameManager.h"
#include "ReadObj.h"

//GLchar* vertexsource, * fragmentsource; // �ҽ��ڵ� ���� ����
//GLuint vertexshader, fragmentshader; // ���̴� ��ü


GLvoid Reshape(int w, int h) {
	glViewport(0, 0, w, h);
}

//--- ���� �����ϰ� ������ �޾ƿ���
GLvoid InitBuffer()
{
	InGameManager::GetInstance().InitBuffer();
}

GLvoid DrawScene() //--- �ݹ� �Լ�: �׸��� �ݹ� �Լ�
{
	cout << "DrawScene GameManager" << endl;
	//--- ����� ���� ����
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	// ������ ���������ο� ���̴� �ҷ�����
		
	InGameManager::GetInstance().DrawScene();
	glEnable(GL_DEPTH_TEST);
	
	glutSwapBuffers(); // ȭ�鿡 ����ϱ�
}

GLvoid InitShader() {
	InGameManager::GetInstance().InitShader();
}

GLvoid InitObject() {
	InGameManager::GetInstance().InitObject();
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
	InitObject();
	InitShader();
	InitBuffer();
	glutDisplayFunc(DrawScene);
	glutReshapeFunc(Reshape);
	glutMainLoop();
}