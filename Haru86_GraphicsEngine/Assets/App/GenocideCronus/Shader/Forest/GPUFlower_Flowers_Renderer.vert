R"(

#version 430

layout(location=0)in vec3 vertex;
layout(location=1)in vec3 normal;
layout(location=2)in vec2 texcoord;
layout(location=3)in vec4 weights;
layout(location=4)in ivec4 joints;

struct StemVertex{
    vec3 vertice;
    vec3 tangent;
    vec3 normal;
    vec3 bioNormal;
    int index;
};

//â‘ÇÃêîÇ»Ç«
struct StemManage{
    float stemLifeVal;
    float stemWaitTime;
    float signNum;
    int manageLifeCountFlag;
    int flowerCount;
    int flowerStartIndex;
    int leafCount;
    int leafStartIndex;
};

//â‘Ç‚åsÇê∂ê¨Ç∑ÇÈÇΩÇﬂÇÃèÓïÒÇç⁄ÇπÇÈç\ë¢ëÃ
struct StemData{
    int resampleIndex;
    int resampleGroupIndex;
    vec3 position;
    vec3 tangent;
    vec3 normal;
    vec3 bioNormal;
    int renderFlag;
    float lifeTime;
};

struct v2g
{
    //vec4 vertex;
    vec2 uv;
    float idInMyStem;
    vec3 nextStemVertex; 
    vec3 tangent;
    vec3 normal;
    vec3 bioNormal;
    vec3 nextTangent;
    vec3 nextNormal;
    vec3 nextBioNormal;
    float lifeTime;
};

uniform mat4 MVPMatrix;
uniform mat4 MMatrix;
uniform mat4 VMatrix;
uniform mat4 PMatrix;
uniform float _time;
uniform float _deltaTime;
uniform vec3 _CameraPos;

out vec2 uv;
out vec3 CameraPos;
out vec3 WorldVertexPos;
out vec3 WorldNormal;

#define PI 3.14159265

float rand(vec2 st){
    return fract(
        sin(dot(st.xy,vec2(12.9898,78.233)))*43758.5453123
    );
}

void main()
{
    int id = gl_InstanceID;
    vec3 randPos = vec3(
        75.0*(rand(vec2(float(id)*100.0,0.951))*2.0-1.0),
        0.0,
        75.0*(rand(vec2(float(id)*100.0,1.294))*2.0-1.0)
    );

    vec4 pos=vec4(vertex,1.0);
    pos.xyz+=randPos;

	gl_Position=MVPMatrix*pos;
	uv=texcoord;
	CameraPos=_CameraPos;
	WorldVertexPos=(MMatrix*pos).xyz;
	WorldNormal=normalize((MMatrix*vec4(normal,0.0)).xyz);
}

)"