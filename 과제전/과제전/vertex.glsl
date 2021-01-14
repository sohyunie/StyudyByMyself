#version 330
//--- in_Position: attribute index 0
//--- in_Color: attribute index 1

layout (location = 0) in vec3 vPos; //--- 위치 속성
// in vec3 in_Color; //--- 색상 속성 조명 들어가면 필요없나...?
layout (location = 1) in vec3 vNormal;

// out vec3 ex_Color; // 프래그먼트 세이더에게 전달
out vec3 FragPos; //--- 객체의 위치값을 프래그먼트 세이더로 보낸다.
out vec3 Normal; //--- 노멀값을 프래그먼트 세이더로 보낸다.


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
