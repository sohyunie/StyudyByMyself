#include "Standard.h"
#include "Block.h"

void Block::DrawObject(GLuint s_program, GLuint VAO, int indexCount) {
	glm::mat4 TR = glm::mat4(1.0f); //--- transformation matrix
	glm::mat4 Rz = glm::mat4(1.0f); //--- rotation matrix
	glm::mat4 Tx = glm::mat4(1.0f); //--- transformation matrix
	Tx = glm::translate(Tx, glm::vec3(0.5, 0.0, 0.0)); //--- x축으로 translation
	Rz = glm::rotate(Rz, glm::radians(45.0f), glm::vec3(0.0f, 1.0f, 0.0f)); //--- z축에대하여 회전
	TR = Rz * Tx; //--- 합성 변환 행렬: translate -> rotate

	glm::vec3 cameraPos = glm::vec3(0.0f, 2.0f, 5.0f);
	glm::vec3 cameraDirection = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
	glm::mat4 view = glm::mat4(1.0f);

	glm::mat4 projection = glm::mat4(1.0f);
	projection = glm::perspective(glm::radians(45.0f), (float)1200 / (float)800, 0.1f, 100.0f);

	unsigned int modelLocation = glGetUniformLocation(s_program, "modelTransform"); //--- 버텍스 세이더에서모델 변환 위치 가져오기
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(TR)); //--- modelTransform 변수에 변환 값 적용하기

	view = glm::lookAt(cameraPos, cameraDirection, cameraUp);
	unsigned int viewLocation = glGetUniformLocation(s_program, "viewTransform");	// 버텍스 사이에서 viewTransform 변수위치
	glUniformMatrix4fv(viewLocation, 1, GL_FALSE, &view[0][0]);	// viewTransform 변수에 변환값 적용하기

	unsigned int projectionLocation = glGetUniformLocation(s_program, "projectionTransform");
	glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, &projection[0][0]);

	int lightPosLocation = glGetUniformLocation(s_program, "lightPos"); //--- lightPos 값 전달: (0.0, 0.0, 5.0);
	glUniform3f(lightPosLocation, 0.0, 0.0, 5.0);

	int lightColorLocation = glGetUniformLocation(s_program, "lightColor"); //--- lightColor 값 전달: (1.0, 1.0, 1.0) 백색
	glUniform3f(lightColorLocation, 1.0, 1.0, 1.0);

	int objColorLocation = glGetUniformLocation(s_program, "objectColor"); //--- object Color값 전달: (1.0, 0.5, 0.3)의 색
	glUniform3f(objColorLocation, 1.0, 1.0, 1.0);

	int ViewLocation = glGetUniformLocation(s_program, "viewPos");
	glUniform3f(ViewLocation, cameraPos.x, cameraPos.y, cameraPos.z);

	// 사용할 VAO 불러오기
	glBindVertexArray(VAO);
	// 삼각형 그리기
	glDrawArrays(GL_TRIANGLES, 0, indexCount);
}