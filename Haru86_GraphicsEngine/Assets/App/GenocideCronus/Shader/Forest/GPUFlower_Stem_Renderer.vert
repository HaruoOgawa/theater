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

layout(std430,binding = 1) buffer stemVertex_buffer
{
    StemVertex[] stemVertex;
} out_stemVertex_buffer;

layout(std430,binding = 2) buffer stemManage_buffer
{
    StemManage[] stemManage;
} out_stemManage_buffer;

struct v2g
{
    vec4 vertex;
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

    StemVertex sVertex=out_stemVertex_buffer[id];
    StemVertex nextVertex=out_stemVertex_buffer[id+1];
                
    //get lifetime
    int stemNodeID=(id-sVertex.index)/_stemVertexCount;
    StemManage sManage=out_stemManage_buffer[stemNodeID];
    float lifeTime=sManage.stemLifeVal;
                
    //nowStem
    v2g_o.vertex = vec4(sVertex.vertice,1.0);
    v2g_o.uv = v.uv;
    v2g_o.idInMyStem=sVertex.index;
    v2g_o.tangent=sVertex.tangent;
    v2g_o.normal=sVertex.normal;
    v2g_o.bioNormal=sVertex.bioNormal;
                
    //nextStem
    v2g_o.nextStemVertex=nextVertex.vertice;
    v2g_o.nextTangent=nextVertex.tangent;
    v2g_o.nextNormal=nextVertex.normal;
    v2g_o.nextBioNormal=nextVertex.bioNormal;

    v2g_o.lifeTime=lifeTime;
}

)"