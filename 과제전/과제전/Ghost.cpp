#include "Standard.h"
#include "Ghost.h"
#include "InGameManager.h"

default_random_engine dreColor((size_t)time(NULL));
normal_distribution <float>uidColor{ 0.0,1.0 };
uniform_int_distribution <int>uidDirection{ 0,3 };

// ������
// ����� ���� �� ����: ��������� �� Ŭ������ � ���� �ϴ��� ���� ���� ����� �Ű� ����Ѱ� �������� �����ϴϱ� ���ǵ��� cpp���ٰ� Ǯ����´�.
Ghost::Ghost() {
	this->type = ObjectType::GHOST;
}

Ghost::Ghost(int i, int j, Vector3 pos, int id) {
	this->id;
	this->type = ObjectType::GHOST;
	this->position = pos;
	this->board_i = i;
	this->board_j = j;
	this->scale = Vector3(0.5,0.5,0.5);
	this->rotate = Vector3(0.0, 1.0, 0.0);
	this->color = glm::vec3(uidColor(dreColor), uidColor(dreColor), uidColor(dreColor));
	this->newDirection = (DIRECTION)uidDirection(dreColor);
	this->boundingOffset = 1.5;
}

void Ghost::SetRandomDirection() {
	this->newDirection = (DIRECTION)uidDirection(dreColor);
}

void Ghost::DrawObject(GLuint s_program) {
	if (this->isActive == false)
		return;
	glm::mat4 STR = glm::mat4(1.0f); //--- transformation matrix
	glm::mat4 R = glm::mat4(1.0f); //--- rotation matrix
	glm::mat4 T = glm::mat4(1.0f); //--- transformation matrix
	glm::mat4 S = glm::mat4(1.0f); //--- rotation matrix
	T = glm::translate(T, this->position.GetGlmVec3(-2)); //--- x������ translation
	R = glm::rotate(R, glm::radians(rotateAngle+=10), this->rotate.GetGlmVec3()); //--- z�࿡���Ͽ� ȸ��
	S = glm::scale(glm::mat4(1.0f), this->scale.GetGlmVec3());
	STR = T * S * R; //--- �ռ� ��ȯ ���: translate -> rotate

	Vector3 lightColor = InGameManager::GetInstance().GetLightColor();
	glm::vec3 cameraPos = InGameManager::GetInstance().GetCameraPos();
	glm::vec3 cameraDirection = InGameManager::GetInstance().GetCameraDirection();
	glm::vec3 cameraUp = InGameManager::GetInstance().GetCameraUp();

	glm::mat4 view = glm::lookAt(glm::vec3(cameraPos), cameraDirection, cameraUp);
	glm::mat4 proj = glm::perspective(glm::radians(60.0f), WINDOW_WITDH / (float)WINDOW_HEIGHT, 0.001f, 1000.f);

	unsigned int modelLocation = glGetUniformLocation(s_program, "g_modelTransform"); //--- ���ؽ� ���̴������� ��ȯ ��ġ ��������
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(STR)); //--- modelTransform ������ ��ȯ �� �����ϱ�

	unsigned int viewLocation = glGetUniformLocation(s_program, "g_view");	// ���ؽ� ���̿��� viewTransform ������ġ
	glUniformMatrix4fv(viewLocation, 1, GL_FALSE, glm::value_ptr(view));	// viewTransform ������ ��ȯ�� �����ϱ�

	unsigned int projectionLocation = glGetUniformLocation(s_program, "g_projection");
	glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, glm::value_ptr(proj));

	int lightPosLocation = glGetUniformLocation(s_program, "g_lightPos"); //--- lightPos �� ����: (0.0, 0.0, 5.0);
	glUniform3f(lightPosLocation, lightPos.x, lightPos.y, lightPos.z);

	int lightColorLocation = glGetUniformLocation(s_program, "g_lightColor"); //--- lightColor �� ����: (1.0, 1.0, 1.0) ���
	glUniform3f(lightColorLocation, lightColor.x, lightColor.y, lightColor.z);

	int objColorLocation = glGetUniformLocation(s_program, "g_objectColor"); //--- object Color�� ����: (1.0, 0.5, 0.3)�� ��
	glUniform3f(objColorLocation, this->color.x, this->color.y, this->color.z);

	int ViewLocation = glGetUniformLocation(s_program, "g_cameraPos");
	glUniform3f(ViewLocation, cameraPos.x, cameraPos.y, cameraPos.z);

	unsigned int flaglocation = glGetUniformLocation(s_program, "flag");
	glUniform1i(flaglocation, 0);

	// ����� VAO �ҷ�����
	glBindVertexArray(InGameManager::GetInstance().GetVAO(this->type));
	// �ﰢ�� �׸���
	glDrawElements(GL_TRIANGLES, InGameManager::GetInstance().GetObjData(this->type)->indexCount, GL_UNSIGNED_INT, 0);
}