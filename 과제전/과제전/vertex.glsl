#version 330

// layout 한정자 : 한정자는 변수나 함수의 형식 인자의 앞에 붙여 해당 변수나 인자의 특성을 결정짓는다.
// 셰이더로 정보를 주고 받을 때는 변수 사용
// in/out: 입력/출력 변수, cpu로부터 데이터를 전달받고 각 셰이더의 연산 결과를 전달할 때 사용 (읽기만 가능)
// 입력변수는 이전 셰이더 스테이지와 연결, 출력변수는 다음 셰이더 스테이지(프레그먼트) 셰이더로 연결 (호출자)
layout (location = 0)in vec3 in_Position;
layout (location = 1)in vec3 vNormal;
layout (location = 2)in vec2 vTexCoord;

out vec3 ex_Color;
out vec2 TexCoord;
out vec3 FragPos;
out vec3 Normal;

// uniform: cpu위의 응용프로그램에서 gpu위의 셰이더로 데이터를 전달하는 방법
// 필요한 셰이더에서 전역변수 형태로 선언후 사용 
// 메인 프로그램에서 변수 설정

// 변환행렬 적용 
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