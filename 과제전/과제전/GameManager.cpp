#include "Standard.h"
#include "InGameManager.h"

//GLchar* vertexsource, * fragmentsource; // �ҽ��ڵ� ���� ����
//GLuint vertexshader, fragmentshader; // ���̴� ��ü


GLvoid Reshape(int w, int h) {
	glViewport(0, 0, w, h);
}

GLvoid Keyboard(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 'a':
		glm::vec3 cameraPos = InGameManager::GetInstance().GetCameraPos();
		InGameManager::GetInstance().SetCamera(glm::vec3(cameraPos.x - 20.0f, cameraPos.y, cameraPos.z));
		break;
	case 'd':		
		cameraPos = InGameManager::GetInstance().GetCameraPos();
		InGameManager::GetInstance().SetCamera(glm::vec3(cameraPos.x + 20.0f, cameraPos.y, cameraPos.z));
		break;
	case 'w':
		cameraPos = InGameManager::GetInstance().GetCameraPos();
		InGameManager::GetInstance().SetCamera(glm::vec3(cameraPos.x, cameraPos.y + 20.0f, cameraPos.z));
		break;
	case 's':
		cameraPos = InGameManager::GetInstance().GetCameraPos();
		InGameManager::GetInstance().SetCamera(glm::vec3(cameraPos.x, cameraPos.y - 20.0f, cameraPos.z));
		break;
	default:
		break;
	}

	glutPostRedisplay();
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

	//if (InGameManager::GetInstance().GetIsDrawFill())
	//	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	//else
	//	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	glPolygonMode(GL_FRONT, GL_FILL);
	glFrontFace(GL_CCW);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	InGameManager::GetInstance().DrawScene();
	
	glutSwapBuffers(); // ȭ�鿡 ����ϱ�
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
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
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(WINDOW_WITDH, WINDOW_HEIGHT);
	glutCreateWindow("Example1");
	//--- GLEW �ʱ�ȭ�ϱ�
	glewExperimental = GL_TRUE;
	glewInit();
	InitObject();
	InitShader();
	InitBuffer();
	glutDisplayFunc(DrawScene);
	glutReshapeFunc(Reshape);
	glutKeyboardFunc(Keyboard);
	glutMainLoop();
}