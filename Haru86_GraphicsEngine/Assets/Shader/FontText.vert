#version 330

uniform mat4 MVPMatrix;
uniform mat4 MMatrix;
uniform mat4 VMatrix;
uniform mat4 PMatrix;
uniform float _time;
uniform float _deltaTime;
uniform vec2 _resolution;

layout(location=0)in vec3 vertex;
layout(location=1)in vec3 normal;
layout(location=2)in vec2 texcoord;

out vec2 uv;
out float time;
out vec2 resolution;

#define rot(a) mat2(cos(a),-sin(a),sin(a),cos(a))

void main(){
	vec4 pos=vec4(vertex,1.0);
	pos.xy*=2.0;
	pos.x*=2.0;
	pos.y*=0.5;
	gl_Position=MVPMatrix*pos;
	time=_time*0.001;
	uv=texcoord;
	resolution=_resolution;
}