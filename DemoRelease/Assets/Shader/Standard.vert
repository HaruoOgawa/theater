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
layout(location=2)in vec2 texcoord;
layout(location=3)in vec4 weights;
layout(location=4)in ivec4 joints;

out vec2 uv;
out vec3 CameraPos;
out vec3 WorldVertexPos;

#define rot(a) mat2(cos(a),-sin(a),sin(a),cos(a))

void main(){
	vec4 pos=vec4(vertex,1.0);

	pos.xy*=rot(_time*0.001);
	pos.xz*=rot(_time*0.001);
	pos.yz*=rot(_time*0.001);

	gl_Position=MVPMatrix*pos;
	uv=texcoord;
	CameraPos=_CameraPos;
	WorldVertexPos=(MMatrix*pos).xyz;
}