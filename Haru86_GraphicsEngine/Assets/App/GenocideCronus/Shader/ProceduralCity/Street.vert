R"(

#version 410

layout(location=0)in vec3 vertex;
layout(location=1)in vec3 normal;
layout(location=2)in vec2 texcoord;

out vec3 v2g_Normal;
out int v2g_id;

uniform mat4 MVPMatrix;
uniform mat4 MMatrix;
uniform mat4 VMatrix;
uniform mat4 PMatrix;
uniform float _time;
uniform float _deltaTime;
uniform vec3 _WorldCameraPos;

void main(){
	// 基本パラメーター
	vec4 WorldPos=MMatrix*vec4(vertex,1.0);
	float pointNum = 1024.0;
	float Segment = 32.0;
	float id =float(gl_InstanceID);
	vec2 domainID=vec2(0.0);
	domainID.y=floor(id/Segment);
	domainID.x=id-domainID.y*Segment;
	domainID=domainID-Segment*0.5;
	float PlaneSize = 50.0;

	// オフセットする
	WorldPos.xyz+=vec3(
		PlaneSize*( domainID.x/(Segment*0.5) ),
		0.0,
		PlaneSize*( domainID.y/(Segment*0.5) )
	);

	// 
	gl_Position=WorldPos;
	v2g_Normal=vec3(0.0,1.0,0.0);
	v2g_id = gl_InstanceID;
}

)"