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

void processSpecialKeys(int key, int x, int y)
{
	Vector3 playerPos = Vector3();
	glm::vec3 playerDir = Vector3().glmvec3();
	switch (key)
	{
	case GLUT_KEY_DOWN:
		cout << "down됨?" << endl;
		playerPos = InGameManager::GetInstance().GetPlayer()->GetPlayerPos();
		InGameManager::GetInstance().GetPlayer()->SetPlayerPos(playerPos + Vector3(0.0, 0.0, -2.0));
		InGameManager::GetInstance().SetCameraPos(playerPos.GetGlmVec3());
		playerDir = InGameManager::GetInstance().GetCameraDirection();
		InGameManager::GetInstance().SetCameraDirection(playerDir);
		break;

	case GLUT_KEY_UP:
		playerPos = InGameManager::GetInstance().GetPlayer()->GetPlayerPos();
		InGameManager::GetInstance().GetPlayer()->SetPlayerPos(playerPos + Vector3(0.0, 0.0, 2.0));
		InGameManager::GetInstance().SetCameraPos(playerPos.GetGlmVec3());
		playerDir = InGameManager::GetInstance().GetCameraDirection();
		InGameManager::GetInstance().SetCameraDirection(playerDir);
		break;

	case GLUT_KEY_LEFT:
		playerPos = InGameManager::GetInstance().GetPlayer()->GetPlayerPos();
		InGameManager::GetInstance().GetPlayer()->SetPlayerPos(playerPos + glm::normalize(glm::cross(glm::vec3(0.0, 0.0, -1.0), glm::vec3(0.0, 1.0, 0.0))));
		InGameManager::GetInstance().SetCameraPos(playerPos.GetGlmVec3());
		playerDir = InGameManager::GetInstance().GetCameraDirection();
		InGameManager::GetInstance().SetCameraDirection(playerDir);


		break;

	case GLUT_KEY_RIGHT:
		playerPos = InGameManager::GetInstance().GetPlayer()->GetPlayerPos();
		InGameManager::GetInstance().GetPlayer()->SetPlayerPos(playerPos - glm::normalize(glm::cross(glm::vec3(0.0, 0.0, -1.0), glm::vec3(0.0, 1.0, 0.0))));
		InGameManager::GetInstance().SetCameraPos(playerPos.GetGlmVec3());
		playerDir = InGameManager::GetInstance().GetCameraDirection();
		InGameManager::GetInstance().SetCameraDirection(playerDir);
		break;
	}

	glutPostRedisplay();
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
	glutSpecialFunc(processSpecialKeys);
	glutMainLoop();
}
