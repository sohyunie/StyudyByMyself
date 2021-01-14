#version 330
//--- ex_Color: ���ؽ� ���̴����� �Է¹޴� ���� ��
//--- gl_FragColor: ����� ������ ������ ���� ���α׷����� ���� ��.
// in vec3 ex_Color; //--- ���ؽ� ���̴����Լ� ���� ����
// out vec4 gl_FragColor; //--- ���� ���

in vec3 FragPos; //--- ��ְ��� ����ϱ� ���� ��ü�� ��ġ���� ���ؽ� ���̴����� �޾ƿ´�.
in vec3 Normal;
out vec4 FragColor;

uniform vec3 lightPos; //--- ������ ��ġ
uniform vec3 lightColor; //--- ������ ��
uniform vec3 objectColor; //--- ��ü�� ��
uniform vec3 viewPos;

void main()
{
	float ambientLight = 0.3;	// �ֺ� ���� ����
	vec3 ambient = ambientLight * lightColor;	// �ֺ� ���� ��

	vec3 normalVector = normalize(Normal);
	vec3 lightDir = normalize(lightPos - FragPos);	// ǥ��� ������ ��ġ�� ������ ������ �����Ѵ�.
	float diffuseLight = max(dot(Normal, lightDir), 0.0);	// N�� L�� ���� ������ ���� ����: ���� ����
	vec3 diffuse = diffuseLight * lightColor;	// ��� �ݻ� ����: ����ݻ簪 * �������

	int shininess = 128;	// ���� ���
	vec3 viewDir = normalize(viewPos- FragPos);	// �������� ����
	vec3 reflectDir = reflect(-lightDir, normalVector);	// �ݻ� ����: reflect �Լ� - �Ի� ������ �ݻ� ���� ���
	float specularLight = max(dot (viewDir, reflectDir), 0.0);	// V�� R�� ���������� ���� ����: ���� ����
	specularLight = pow(specularLight, shininess);	// shininess ���� ���־� ���̶���Ʈ�� ������ش�.
	vec3 specular = specularLight * lightColor;	// �ſ� �ݻ� ����: �ſ�ݻ簪 * �������

	vec3 result = (ambient + diffuse + specular) * objectColor; // ���� ���� ������ �ȼ� ����: (�ֺ�+����ݻ�+�ſ�ݻ�����) * ��ü ����

	FragColor = vec4 (result, 1.0);	// �ȼ� ���� ���
}