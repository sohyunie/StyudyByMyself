#include "InGameManager.h"

InGameManager* InGameManager::instance = NULL;

GLchar* vertexsource, * fragmentsource; // �ҽ��ڵ� ���� ����
GLuint vertexshader, fragmentshader; // ���̴� ��ü

GLuint VAO, VBO;

char* filetobuf(const char* file)
{
	FILE* fptr;
	long length;
	char* buf;
	fptr = fopen(file, "rb"); // Open file for reading
	if (!fptr) // Return NULL on failure
		return NULL;
	fseek(fptr, 0, SEEK_END); // Seek to the end of the file
	length = ftell(fptr); // Find out how many bytes into the file we are
	buf = (char*)malloc(length + 1); // Allocate a buffer for the entire length of the file and a null terminator
	fseek(fptr, 0, SEEK_SET); // Go back to the beginning of the file
	fread(buf, length, 1, fptr); // Read the contents of the file in to the buffer
	fclose(fptr); // Close the file
	buf[length] = 0; // Null terminator
	return buf; // Return the buffer
}
void make_vertexShader()
{
	vertexsource = filetobuf("vertex.glsl");
	//--- ���ؽ� ���̴� ��ü �����
	vertexshader = glCreateShader(GL_VERTEX_SHADER);
	//--- ���̴� �ڵ带 ���̴� ��ü�� �ֱ�
	glShaderSource(vertexshader, 1, (const GLchar**)&vertexsource, 0);
	//--- ���ؽ� ���̴� �������ϱ�
	glCompileShader(vertexshader);
	//--- �������� ����� ���� ���� ���: ���� üũ
	GLint result;
	GLchar errorLog[512];
	glGetShaderiv(vertexshader, GL_COMPILE_STATUS, &result);
	if (!result)
	{
		glGetShaderInfoLog(vertexshader, 512, NULL, errorLog);
		cerr << "ERROR: vertex shader ������ ����\n" << errorLog << endl;
		return;
	}
}

void make_fragmentShader()
{
	fragmentsource = filetobuf("fragment.glsl");
	//--- �����׸�Ʈ ���̴� ��ü �����
	fragmentshader = glCreateShader(GL_FRAGMENT_SHADER);
	//--- ���̴� �ڵ带 ���̴� ��ü�� �ֱ�
	glShaderSource(fragmentshader, 1, (const GLchar**)&fragmentsource, 0);
	//--- �����׸�Ʈ ���̴� ������
	glCompileShader(fragmentshader);
	//--- �������� ����� ���� ���� ���: ������ ���� üũ
	GLint result;
	GLchar errorLog[512];
	glGetShaderiv(fragmentshader, GL_COMPILE_STATUS, &result);
	if (!result)
	{
		glGetShaderInfoLog(fragmentshader, 512, NULL, errorLog);
		cerr << "ERROR: fragment shader ������ ����\n" << errorLog << endl;
		return;
	}
}

GLuint s_program;
//--- ���̴� ���α׷� �����ϱ�
void InitShader()
{
	cout << "InitShader GameManager" << endl;
	make_vertexShader();
	make_fragmentShader();
	//-- shader Program
	s_program = glCreateProgram();
	glAttachShader(s_program, vertexshader);
	glAttachShader(s_program, fragmentshader);
	glLinkProgram(s_program);
	//--- ���̴� �����ϱ�
	glDeleteShader(vertexshader);
	glDeleteShader(fragmentshader);
	//--- Shader Program ����ϱ�
	glUseProgram(s_program);
}

GLvoid InGameManager::DrawScene() {
	glUseProgram(s_program);
	cout << "DrawScene InGameManager" << endl;
	glm::mat4 TR = glm::mat4(1.0f); //--- transformation matrix
	glm::mat4 Rz = glm::mat4(1.0f); //--- rotation matrix
	glm::mat4 Tx = glm::mat4(1.0f); //--- transformation matrix
	Tx = glm::translate(Tx, glm::vec3(0.5, 0.0, 0.0)); //--- x������ translation
	Rz = glm::rotate(Rz, glm::radians(45.0f), glm::vec3(0.0f, 1.0f, 0.0f)); //--- z�࿡���Ͽ� ȸ��
	TR = Rz * Tx; //--- �ռ� ��ȯ ���: translate -> rotate

	glm::vec3 cameraPos = glm::vec3(0.0f, 2.0f, 5.0f);
	glm::vec3 cameraDirection = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
	glm::mat4 view = glm::mat4(1.0f);

	glm::mat4 projection = glm::mat4(1.0f);
	projection = glm::perspective(glm::radians(45.0f), (float)1200 / (float)800, 0.1f, 100.0f);

	unsigned int modelLocation = glGetUniformLocation(s_program, "modelTransform"); //--- ���ؽ� ���̴������� ��ȯ ��ġ ��������
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(TR)); //--- modelTransform ������ ��ȯ �� �����ϱ�

	view = glm::lookAt(cameraPos, cameraDirection, cameraUp);
	unsigned int viewLocation = glGetUniformLocation(s_program, "viewTransform");	// ���ؽ� ���̿��� viewTransform ������ġ
	glUniformMatrix4fv(viewLocation, 1, GL_FALSE, &view[0][0]);	// viewTransform ������ ��ȯ�� �����ϱ�

	unsigned int projectionLocation = glGetUniformLocation(s_program, "projectionTransform");
	glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, &projection[0][0]);

	int lightPosLocation = glGetUniformLocation(s_program, "lightPos"); //--- lightPos �� ����: (0.0, 0.0, 5.0);
	glUniform3f(lightPosLocation, 0.0, 0.0, 5.0);

	int lightColorLocation = glGetUniformLocation(s_program, "lightColor"); //--- lightColor �� ����: (1.0, 1.0, 1.0) ���
	glUniform3f(lightColorLocation, 1.0, 1.0, 1.0);

	int objColorLocation = glGetUniformLocation(s_program, "objectColor"); //--- object Color�� ����: (1.0, 0.5, 0.3)�� ��
	glUniform3f(objColorLocation, 1.0, 1.0, 1.0);

	int ViewLocation = glGetUniformLocation(s_program, "viewPos");
	glUniform3f(ViewLocation, cameraPos.x, cameraPos.y, cameraPos.z);

	// ����� VAO �ҷ�����
	glBindVertexArray(VAO);
	// �ﰢ�� �׸���
	glDrawArrays(GL_TRIANGLES, 0, 36);
}

GLvoid InGameManager::InitBuffer() {
	cout << "InitBuffer InGameManager" << endl;
	// PLAYER

	// GHOST

	// BLOCK
	glGenVertexArrays(1, &VAO); //--- VAO �� �����ϰ� �Ҵ��ϱ�
	glBindVertexArray(VAO); //--- VAO�� ���ε��ϱ�
	glGenBuffers(2, &VBO); //--- 2���� VBO�� �����ϰ� �Ҵ��ϱ�
	//--- 1��° VBO�� Ȱ��ȭ�Ͽ� ���ε��ϰ�, ���ؽ� �Ӽ� (��ǥ��)�� ����
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(blockVertex), blockVertex, GL_STATIC_DRAW);
	// ��ǥ���� attribute �ε��� 0���� ����Ѵ�: ���ؽ� �� 3* float
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	// attribute �ε��� 0���� ��밡���ϰ� ��
	glEnableVertexAttribArray(0);
	// ������ attribute �ε��� 1���� ����Ѵ�: ���ؽ� �� 3*float
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	// attribute �ε��� 1���� ��� �����ϰ� ��.
	glEnableVertexAttribArray(1);
}