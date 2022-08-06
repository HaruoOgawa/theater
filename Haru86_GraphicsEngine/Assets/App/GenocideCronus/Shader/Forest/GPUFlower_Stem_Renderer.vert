R"(

#version 430

layout(location=0)in vec3 vertex;
layout(location=1)in vec3 normal;
layout(location=2)in vec2 texcoord;
layout(location=3)in vec4 weights;
layout(location=4)in ivec4 joints;

struct StemVertex{
    vec4 vertice;
    vec4 tangent;
    vec4 normal;
    vec4 bioNormal;
    int index;
};

//花の数など
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

//花や茎を生成するための情報を載せる構造体
struct StemData{
    int resampleIndex;
    int resampleIndexInStem;
    vec4 position;
    vec4 tangent;
    vec4 normal;
    vec4 bioNormal;
    int renderFlag;
    float lifeTime;
    float flowerSize;
};

// 各花の生える基本の座標
struct StemBasePosition {
    vec4 position;
};

layout(std430,binding = 1) buffer stemVertex_buffer
{
    StemVertex stemVertex[];
} out_stemVertex_buffer;

layout(std430,binding = 2) buffer stemManage_buffer
{
    StemManage stemManage[];
} out_stemManage_buffer;

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

uniform int _stemVertexCount;
uniform int _stemSegments;
uniform float _stemRadius;

out v2g v2g_o;

void main(){
    int id=gl_InstanceID;

    StemVertex sVertex=out_stemVertex_buffer.stemVertex[id];
    StemVertex nextVertex=out_stemVertex_buffer.stemVertex[id+1];
                
    //get lifetime
    int stemNodeID=(id-sVertex.index)/_stemVertexCount;
    StemManage sManage=out_stemManage_buffer.stemManage[stemNodeID];
    float lifeTime=sManage.stemLifeVal;
                
    //nowStem
    gl_Position = vec4(sVertex.vertice.xyz,1.0);
    v2g_o.uv = texcoord;
    v2g_o.idInMyStem=sVertex.index;
    v2g_o.tangent=sVertex.tangent.xyz;
    v2g_o.normal=sVertex.normal.xyz;
    v2g_o.bioNormal=sVertex.bioNormal.xyz;
                
    //nextStem
    v2g_o.nextStemVertex=nextVertex.vertice.xyz;
    v2g_o.nextTangent=nextVertex.tangent.xyz;
    v2g_o.nextNormal=nextVertex.normal.xyz;
    v2g_o.nextBioNormal=nextVertex.bioNormal.xyz;

    v2g_o.lifeTime=lifeTime;
}

)"