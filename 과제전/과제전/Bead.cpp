#include "Standard.h"
#include "InGameManager.h"
#include "Bead.h"

default_random_engine dreColor_bead((size_t)time(NULL));
normal_distribution <float>uidColor_bead{ 0.0,1.0 };

Bead::Bead() {
	this->type = ObjectType::BEAD;
}

Bead::Bead(Vector3 pos) {
	this->type = ObjectType::BEAD;
	this->position = Vector3(pos.x, pos.y, pos.z);
	this->scale = Vector3(0.7, 0.7, 0.7);
	this->rotate = Vector3(0.0, 1.0, 0.0);
	this->color = Vector3(1.0, 1.0, 1.0);
	this->boundingOffset = 1.5;
}

GLvoid Bead::DrawObject(GLuint s_program) {
	if (InGameManager::GetInstance().GetPresence() == true) {
		this->color = glm::vec3(uidColor_bead(dreColor_bead), uidColor_bead(dreColor_bead), uidColor_bead(dreColor_bead));
		glm::mat4 STR = glm::mat4(1.0f); //--- transformation matrix
		glm::mat4 R = glm::mat4(1.0f); //--- rotation matrix
		glm::mat4 T = glm::mat4(1.0f); //--- transformation matrix
		glm::mat4 S = glm::mat4(1.0f);
		T = glm::translate(T, this->position.GetGlmVec3()); //--- x축으로 translation
		R = glm::rotate(R, glm::radians(45.0f), this->rotate.GetGlmVec3()); //--- z축에대하여 회전
		S = glm::scale(glm::mat4(1.0f), this->scale.GetGlmVec3());
		STR = T * S * R; //--- 합성 변환 행렬: translate -> rotate

		glm::vec3 cameraPos = InGameManager::GetInstance().GetCameraPos();
		glm::vec3 cameraDirection = InGameManager::GetInstance().GetCameraDirection();
		glm::vec3 cameraUp = InGameManager::GetInstance().GetCameraUp();

		glm::mat4 view = glm::lookAt(glm::vec3(cameraPos), cameraDirection, cameraUp);
		glm::mat4 proj = glm::perspective(glm::radians(60.0f), WINDOW_WITDH / (float)WINDOW_HEIGHT, 0.001f, 1000.f);

		unsigned int modelLocation = glGetUniformLocation(s_program, "g_modelTransform"); //--- 버텍스 세이더에서모델 변환 위치 가져오기
		glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(STR)); //--- modelTransform 변수에 변환 값 적용하기

		unsigned int viewLocation = glGetUniformLocation(s_program, "g_view");	// 버텍스 사이에서 viewTransform 변수위치
		glUniformMatrix4fv(viewLocation, 1, GL_FALSE, glm::value_ptr(view));	// viewTransform 변수에 변환값 적용하기

		unsigned int projectionLocation = glGetUniformLocation(s_program, "g_projection");
		glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, glm::value_ptr(proj));

		int lightPosLocation = glGetUniformLocation(s_program, "g_lightPos"); //--- lightPos 값 전달: (0.0, 0.0, 5.0);
		glUniform3f(lightPosLocation, 0.0, 10.0, 0.0);

		int lightColorLocation = glGetUniformLocation(s_program, "g_lightColor"); //--- lightColor 값 전달: (1.0, 1.0, 1.0) 백색
		glUniform3f(lightColorLocation, 1.0, 1.0, 1.0);

		int objColorLocation = glGetUniformLocation(s_program, "g_objectColor"); //--- object Color값 전달: (1.0, 0.5, 0.3)의 색
		glUniform3f(objColorLocation, this->color.x, this->color.y, this->color.z);

		int ViewLocation = glGetUniformLocation(s_program, "g_cameraPos");
		glUniform3f(ViewLocation, cameraPos.x, cameraPos.y, cameraPos.z);


		// 사용할 VAO 불러오기
		glBindVertexArray(InGameManager::GetInstance().GetVAO(this->type));
		// 삼각형 그리기
		glDrawElements(GL_TRIANGLES, InGameManager::GetInstance().GetObjData(this->type)->indexCount, GL_UNSIGNED_INT, 0);
	}
}