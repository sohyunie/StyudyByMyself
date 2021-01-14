#include "Standard.h"
#include "InGameManager.h"
#include "ReadObj.h"

//GLchar* vertexsource, * fragmentsource; // 소스코드 저장 변수
//GLuint vertexshader, fragmentshader; // 세이더 객체


GLvoid Reshape(int w, int h) {
	glViewport(0, 0, w, h);
}

//--- 버퍼 생성하고 데이터 받아오기
GLvoid InitBuffer()
{
	InGameManager::GetInstance().InitBuffer();
}

GLvoid DrawScene() //--- 콜백 함수: 그리기 콜백 함수
{
	cout << "DrawScene GameManager" << endl;
	//--- 변경된 배경색 설정
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	// 렌더링 파이프라인에 세이더 불러오기
		
	InGameManager::GetInstance().DrawScene();
	glEnable(GL_DEPTH_TEST);
	
	glutSwapBuffers(); // 화면에 출력하기
}

GLvoid InitShader() {
	InGameManager::GetInstance().InitShader();
}

GLvoid InitObject() {
	InGameManager::GetInstance().InitObject();
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
	InitObject();
	InitShader();
	InitBuffer();
	glutDisplayFunc(DrawScene);
	glutReshapeFunc(Reshape);
	glutMainLoop();
}