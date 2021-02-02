#pragma once
#include "DynamicObject.h"

class Ghost : public DynamicObject
{
private:
	bool isActive = false;
public:
	Ghost();
	Ghost(Vector3 pos);
	virtual void DrawObject(GLuint s_program) override;
	void MakeGhost(Ghost ghost);
	bool GetIsActive() { return this->isActive; }
	glm::vec3 color;
	void SetIsActive(bool isOn) { this->isActive = isOn; }


	/*void SetGhostColor(glm::vec3 ghostColor) {
		this->color = ghostColor;
	}*/
};
