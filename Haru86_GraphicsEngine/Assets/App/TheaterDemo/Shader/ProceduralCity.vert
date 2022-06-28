R"(

#version 430

uniform float _time;
uniform float _deltaTime;
uniform vec3 _CameraPos;

layout(location=0)in vec3 vertex;
layout(location=1)in vec3 normal;
layout(location=2)in vec2 texcoord;
layout(location=3)in vec4 weights;
layout(location=4)in ivec4 joints;

struct v2g{
	vec4 position;
	vec2 uv;
	vec3 worldPos;
};

out v2g v2g_o; 

void main(){
	vec4 pos=vec4(vertex,1.0);

	gl_Position=pos;
	v2g_o.position=pos;
	v2g_o.uv=texcoord;
	v2g_o.worldPos=pos.xyz;
}

)"