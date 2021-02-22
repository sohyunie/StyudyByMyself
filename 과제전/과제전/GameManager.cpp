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
	glutPostRedisplay();
	glutSwapBuffers();

	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
}

GLvoid Reshape(int w, int h)
{
	glViewport(0, 0, w, h);
}

GLvoid Keyboard(unsigned char key, int x, int y)
{
	float cameraSpeed = 0.05f;
	Vector3 playerPos;
	switch (key)
	{
	case ((char)13):	// enter key
		if (InGameManager::GetInstance().GetState() == GAMESTATE::LOBBY)
			InGameManager::GetInstance().SetState(GAMESTATE::INGAME);
		break;
	case '1':
		InGameManager::GetInstance().SetFPS(true);
		break;
	case '3':
		InGameManager::GetInstance().SetFPS(false);
		break;
	case ' ':	// space bar
		if (InGameManager::GetInstance().GetState() == GAMESTATE::GAMEOVER ||
			InGameManager::GetInstance().GetState() == GAMESTATE::CLEAR) {
			InGameManager::GetInstance().InitGame();
			InGameManager::GetInstance().SetState(GAMESTATE::LOBBY);
		}
		break;
	}

	glutPostRedisplay();
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
	switch (key)
	{
	case GLUT_KEY_RIGHT:
		switch (InGameManager::GetInstance().GetPlayer()->newDirection) {
		case DIRECTION::DIR_NONE:
			InGameManager::GetInstance().GetPlayer()->newDirection = DIRECTION::UP;
			break;
		case DIRECTION::UP:
			InGameManager::GetInstance().GetPlayer()->newDirection = DIRECTION::LEFT;
			break;
		case DIRECTION::LEFT:
			InGameManager::GetInstance().GetPlayer()->newDirection = DIRECTION::DOWN;
			break;
		case DIRECTION::DOWN:
			InGameManager::GetInstance().GetPlayer()->newDirection = DIRECTION::RIGHT;
			break;
		case DIRECTION::RIGHT:
			InGameManager::GetInstance().GetPlayer()->newDirection = DIRECTION::UP;
			break;
		}
		break;
	case GLUT_KEY_LEFT:
		switch (InGameManager::GetInstance().GetPlayer()->newDirection) {
		case DIRECTION::DIR_NONE:
			InGameManager::GetInstance().GetPlayer()->newDirection = DIRECTION::UP;
			break;
		case DIRECTION::UP:
			InGameManager::GetInstance().GetPlayer()->newDirection = DIRECTION::RIGHT;
			break;
		case DIRECTION::RIGHT:
			InGameManager::GetInstance().GetPlayer()->newDirection = DIRECTION::DOWN;
			break;
		case DIRECTION::DOWN:
			InGameManager::GetInstance().GetPlayer()->newDirection = DIRECTION::LEFT;
			break;
		case DIRECTION::LEFT:
			InGameManager::GetInstance().GetPlayer()->newDirection = DIRECTION::UP;
			break;
		}
		break;
	}

	glutPostRedisplay();
}

void TimerFunction(int value) {
	InGameManager::GetInstance().TimerFunction();

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
