R"(

#version 330

uniform mat4 MVPMatrix;
uniform mat4 MMatrix;
uniform mat4 VMatrix;
uniform mat4 PMatrix;
uniform float _time;
uniform float _deltaTime;
uniform vec3 _CameraPos;

layout(location=0)in vec3 vertex;
layout(location=1)in vec3 normal;
layout(location=2)in vec3 billinfo;

out vec2 uv;
out vec3 CameraPos;
out vec3 WorldVertexPos;
out vec3 WorldNormal;
out vec3 v2f_billinfo;

#define rot(a) mat2(cos(a),-sin(a),sin(a),cos(a))

void main(){
	vec4 pos=vec4(vertex,1.0);

	gl_Position=MVPMatrix*pos;
	CameraPos=_CameraPos;
	WorldVertexPos=(MMatrix*pos).xyz;
	WorldNormal=normalize((MMatrix*vec4(normal,0.0)).xyz);
	v2f_billinfo=billinfo;
}

)"