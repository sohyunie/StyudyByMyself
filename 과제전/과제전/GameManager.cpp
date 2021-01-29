#include "Standard.h"
#include "InGameManager.h"
#include "Player.h"

using namespace std;

void InitBuffer()
{
	InGameManager::GetInstance().InitBuffer();
}

void InitShader()
{
	InGameManager::GetInstance().InitShader();
}


GLvoid drawScene()
{
	glClearColor(0, 0, 0, 1);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	glFrontFace(GL_CCW);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	InGameManager::GetInstance().DrawScene();
	glutSwapBuffers();                        // 화면에 출력하기

	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
}

GLvoid Reshape(int w, int h)
{
	glViewport(0, 0, w, h);
}

GLvoid Keyboard(unsigned char key, int x, int y)
{
	//float cameraSpeed = 0.05f;
	//Vector3 playerPos;
	//switch (key)
	//{
	//case 'x':
	//	glm::vec3 cameraPos = InGameManager::GetInstance().GetCameraPos();
	//	InGameManager::GetInstance().SetCamera(glm::vec3(cameraPos.x - 20.0f, cameraPos.y, cameraPos.z));
	//	break;
	//case 'X':
	//	cameraPos = InGameManager::GetInstance().GetCameraPos();
	//	InGameManager::GetInstance().SetCamera(glm::vec3(cameraPos.x + 20.0f, cameraPos.y, cameraPos.z));
	//	break;
	//case 'y':
	//	cameraPos = InGameManager::GetInstance().GetCameraPos();
	//	InGameManager::GetInstance().SetCamera(glm::vec3(cameraPos.x, cameraPos.y - 20.0f, cameraPos.z));
	//	break;
	//case 'Y':
	//	cameraPos = InGameManager::GetInstance().GetCameraPos();
	//	InGameManager::GetInstance().SetCamera(glm::vec3(cameraPos.x, cameraPos.y + 20.0f, cameraPos.z));
	//	break;
	//case 'z':
	//	cameraPos = InGameManager::GetInstance().GetCameraPos();
	//	InGameManager::GetInstance().SetCamera(glm::vec3(cameraPos.x, cameraPos.y, cameraPos.z - 20.0f));
	//	break;
	//case 'Z':
	//	cameraPos = InGameManager::GetInstance().GetCameraPos();
	//	InGameManager::GetInstance().SetCamera(glm::vec3(cameraPos.x, cameraPos.y, cameraPos.z + 20.0f));
	//	break;
	//}

	//glutPostRedisplay();
}

void releaseKey(int key, int x, int y) {

	switch (key)
	{
	case GLUT_KEY_LEFT:
	case GLUT_KEY_RIGHT: InGameManager::GetInstance().GetPlayer()->deltaAngle = 0; break;

	case GLUT_KEY_UP:
	case GLUT_KEY_DOWN: InGameManager::GetInstance().GetPlayer()->deltaMove = 0; break;
	}
}

void processSpecialKeys(int key, int x, int y)
{
	Vector3 playerPos = Vector3();
	glm::vec3 playerDir = Vector3().glmvec3();
	switch (key)
	{
	case GLUT_KEY_DOWN:
		InGameManager::GetInstance().GetPlayer()->deltaMove = -1.0f * InGameManager::GetInstance().GetDeltaTime();
		break;
	case GLUT_KEY_UP:
		InGameManager::GetInstance().GetPlayer()->deltaMove = 1.0f * InGameManager::GetInstance().GetDeltaTime();
		break;
	case GLUT_KEY_LEFT:
		InGameManager::GetInstance().GetPlayer()->deltaAngle = -0.0005f *InGameManager::GetInstance().GetDeltaTime();
		break;
	case GLUT_KEY_RIGHT:
		InGameManager::GetInstance().GetPlayer()->deltaAngle = 0.0005f * InGameManager::GetInstance().GetDeltaTime();
		break;
	}

	glutPostRedisplay();
}

void TimerFunction(int value) {
	InGameManager::GetInstance().GetTime();
	InGameManager::GetInstance().CalculateTime();

	if (InGameManager::GetInstance().GetPlayer()->deltaAngle)
		InGameManager::GetInstance().computeDir();
	if (InGameManager::GetInstance().GetPlayer()->deltaMove)
		InGameManager::GetInstance().computePos();

	InGameManager::GetInstance().CameraSetting();

	glutTimerFunc(10, TimerFunction, 1);
}

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(WINDOW_WITDH, WINDOW_HEIGHT);
	glutCreateWindow("Example1");

	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK)
	{
		std::cerr << "Unable to initialize GLEW" << std::endl;
		exit(EXIT_FAILURE);
	}
	else
		std::cout << "GLEW Initialized\n";

	//ReadObj(FILE_NAME, InGameManager::GetInstance().gobj->vPosData, InGameManager::GetInstance().gobj->vNormalData, InGameManager::GetInstance().gobj->vTextureCoordinateData, InGameManager::GetInstance().gobj->indexData, InGameManager::GetInstance().gobj->vertexCount, InGameManager::GetInstance().gobj->indexCount);
	InGameManager::GetInstance().InitObject();
	InitShader();
	InitBuffer();

	glutDisplayFunc(drawScene);
	glutReshapeFunc(Reshape);
	glutKeyboardFunc(Keyboard);
	glutSpecialUpFunc(releaseKey);
	glutSpecialFunc(processSpecialKeys);
	glutTimerFunc(50, TimerFunction, 1);
	glutMainLoop();
}
