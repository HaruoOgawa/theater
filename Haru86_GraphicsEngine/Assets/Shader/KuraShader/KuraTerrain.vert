#version 410

uniform mat4 MVPMatrix;
uniform mat4 MMatrix;
uniform mat4 VMatrix;
uniform mat4 PMatrix;
uniform float _time;
uniform float _deltaTime;

layout(location=0)in vec3 vertex;
layout(location=1)in vec3 normal;
layout(location=2)in vec2 texcoord;

//out vec2 uv;
out vec3 v2tesc_normal;

#define rot(a) mat2(cos(a),-sin(a),sin(a),cos(a))
#define PI 3.14159265

void main(){
	vec4 pos=vec4(vertex,1.0);
	//pos.xz*=rot(_time*0.001);
	//pos.xy*=rot(_time*0.001);
	//pos.yz*=rot(_time*0.001);
	
	//pos.yz*=rot(PI/6.0);
	
	gl_Position=pos;
	v2tesc_normal=(vec4(normal,1.0)).xyz;
}