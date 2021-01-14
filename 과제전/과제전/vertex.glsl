#version 330
//--- in_Position: attribute index 0
//--- in_Color: attribute index 1

layout (location = 0) in vec3 vPos; //--- ��ġ �Ӽ�
// in vec3 in_Color; //--- ���� �Ӽ� ���� ���� �ʿ����...?
layout (location = 1) in vec3 vNormal;

// out vec3 ex_Color; // �����׸�Ʈ ���̴����� ����
out vec3 FragPos; //--- ��ü�� ��ġ���� �����׸�Ʈ ���̴��� ������.
out vec3 Normal; //--- ��ְ��� �����׸�Ʈ ���̴��� ������.


uniform mat4 modelTransform;
uniform mat4 viewTransform;
uniform mat4 projectionTransform;

void main(void)
{
	gl_Position =  projectionTransform * viewTransform * modelTransform * vec4 (vPos, 1.0);
	FragPos = vec3(modelTransform * vec4(vPos, 1.0));
	Normal = vNormal;
	// ex_Color = in_Color;
}
