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
	vec3 randPos;
	int id;
};

out v2g v2g_o; 

float rand(vec2 seeds){
  return fract(sin(dot(seeds, vec2(12.9898,78.233))) * 43758.5453);
}

void main(){
	int id=gl_InstanceID;
	float id_f=float(id);
	vec2 domainID=vec2(0.0);
	domainID.y=floor(id_f/32.0);
	domainID.x=id_f-domainID.y*32.0;
	
	vec3 randPos=vec3(0.0);
	//randPos.xz=( vec2( rand( vec2(float(gl_InstanceID),0.321)),rand( vec2(float(gl_InstanceID),0.741)) ) * 2.0-1.0 )*25.0;
	//randPos.xz=domainID-32.0*0.5;

	vec4 pos=vec4(vertex,1.0);

	gl_Position=pos;
	v2g_o.position=pos;
	v2g_o.uv=texcoord;
	v2g_o.randPos=randPos;
	v2g_o.id=id;
}

)"