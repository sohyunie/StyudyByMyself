#version 330

// layout ������ : �����ڴ� ������ �Լ��� ���� ������ �տ� �ٿ� �ش� ������ ������ Ư���� �������´�.
// ���̴��� ������ �ְ� ���� ���� ���� ���
// in/out: �Է�/��� ����, cpu�κ��� �����͸� ���޹ް� �� ���̴��� ���� ����� ������ �� ��� (�б⸸ ����)
// �Էº����� ���� ���̴� ���������� ����, ��º����� ���� ���̴� ��������(�����׸�Ʈ) ���̴��� ���� (ȣ����)
layout (location = 0)in vec3 in_Position;
layout (location = 1)in vec3 vNormal;
layout (location = 2)in vec2 vTexCoord;

out vec3 ex_Color;
out vec2 TexCoord;
out vec3 FragPos;
out vec3 Normal;

// uniform: cpu���� �������α׷����� gpu���� ���̴��� �����͸� �����ϴ� ���
// �ʿ��� ���̴����� �������� ���·� ������ ��� 
// ���� ���α׷����� ���� ����

// ��ȯ��� ���� 
uniform mat4 Transform;
uniform mat4 View;
uniform mat4 Projection;
uniform vec3 in_Color;


void main()
{
	gl_Position = Projection * View * Transform * vec4 (in_Position.x, in_Position.y, in_Position.z, 1.0);
	ex_Color = in_Color;
	TexCoord = vTexCoord;
	FragPos = vec3(Transform * vec4(in_Position, 1.0));
	Normal = vNormal;
}