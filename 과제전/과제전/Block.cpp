#include "Standard.h"
#include "Block.h"
#include "InGameManager.h"

void Block::DrawObject(GLuint s_program, GLuint VAO, int indexCount) {
	//cout << "DrawObject : Block" << endl; 
	glm::mat4 TR = glm::mat4(1.0f); //--- transformation matrix
	glm::mat4 Rz = glm::mat4(1.0f); //--- rotation matrix
	glm::mat4 Tx = glm::mat4(1.0f); //--- transformation matrix
	Tx = glm::translate(Tx, glm::vec3(this->position.x, this->position.y, this->position.z)); //--- x������ translation
	Rz = glm::rotate(Rz, glm::radians(45.0f), glm::vec3(0.0f, 1.0f, 0.0f)); //--- z�࿡���Ͽ� ȸ��
	TR = Rz * Tx; //--- �ռ� ��ȯ ���: translate -> rotate

	glm::vec3 cameraPos = InGameManager::GetInstance().GetCameraPos();
	glm::vec3 cameraDirection = InGameManager::GetInstance().GetCameraDirection();
	glm::vec3 cameraUp = InGameManager::GetInstance().GetCameraUp();
	glm::mat4 view = glm::mat4(1.0f);
	glm::mat4 projection = glm::perspective(glm::radians(60.0f), WINDOW_WITDH / (float)WINDOW_HEIGHT, 0.001f, 1000.f);

	unsigned int modelLocation = glGetUniformLocation(s_program, "g_modelTransform"); //--- ���ؽ� ���̴������� ��ȯ ��ġ ��������
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(TR)); //--- modelTransform ������ ��ȯ �� �����ϱ�

	view = glm::lookAt(cameraPos, cameraDirection, cameraUp);
	unsigned int viewLocation = glGetUniformLocation(s_program, "g_view");	// ���ؽ� ���̿��� viewTransform ������ġ
	glUniformMatrix4fv(viewLocation, 1, GL_FALSE, &view[0][0]);	// viewTransform ������ ��ȯ�� �����ϱ�

	unsigned int projectionLocation = glGetUniformLocation(s_program, "g_projection");
	glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, &projection[0][0]);

	int lightPosLocation = glGetUniformLocation(s_program, "g_lightPos"); //--- lightPos �� ����: (0.0, 0.0, 5.0);
	glUniform3f(lightPosLocation, 0.0, 0.0, 5.0);

	int lightColorLocation = glGetUniformLocation(s_program, "g_lightColor"); //--- lightColor �� ����: (1.0, 1.0, 1.0) ���
	glUniform3f(lightColorLocation, 1.0, 1.0, 1.0);

	int objColorLocation = glGetUniformLocation(s_program, "g_objectColor"); //--- object Color�� ����: (1.0, 0.5, 0.3)�� ��
	glUniform3f(objColorLocation, 1.0, 1.0, 1.0);

	int ViewLocation = glGetUniformLocation(s_program, "g_cameraPos");
	glUniform3f(ViewLocation, cameraPos.x, cameraPos.y, cameraPos.z);

	// ����� VAO �ҷ�����
	glBindVertexArray(VAO);
	// �ﰢ�� �׸���
	glDrawArrays(GL_TRIANGLES, 0, indexCount);
}