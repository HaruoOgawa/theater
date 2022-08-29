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
    int resampleIndexInStem;
    vec4 position;
    vec4 tangent;
    vec4 normal;
    vec4 bioNormal;
    int renderFlag;
    float lifeTime;
    float flowerSize;
};

// äeâ‘ÇÃê∂Ç¶ÇÈäÓñ{ÇÃç¿ïW
struct StemBasePosition {
    vec4 position;
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

//StructuredBuffer<StemData> _read_stemDataFlower_buffer;

layout(std430,binding = 4) buffer stemDataFlower_buffer
{
    StemData stemData[];
} out_stemDataFlower_buffer;

out vec2 uv;
out vec3 CameraPos;
out vec3 WorldVertexPos;
out vec3 WorldNormal;
flat out float v2f_id;

#define PI 3.14159265

float rand(vec2 st){
    return fract(
        sin(dot(st.xy,vec2(12.9898,78.233)))*43758.5453123
    );
}

void main()
{
    int id = gl_InstanceID;
    
    StemData data=out_stemDataFlower_buffer.stemData[id];

    vec4 pos=vec4(vertex,1.0);
    float l=length(pos.xyz);
    pos.xz*=data.lifeTime*0.8+0.2;
    pos.y*=data.lifeTime*0.3+0.7;
    pos.xyz*=data.flowerSize;
                
    pos.xyz=
        mat3(
            data.normal.x,data.tangent.x,data.bioNormal.x,
            data.normal.y,data.tangent.y,data.bioNormal.y,
            data.normal.z,data.tangent.z,data.bioNormal.z
        ) * pos.xyz + data.position.xyz;

    gl_Position = MVPMatrix*pos;
    uv=texcoord;
    WorldVertexPos=(MMatrix*pos).xyz;
    CameraPos=_CameraPos;
    // Ç»ÇÒÇ©ñ@ê¸Ç™ãtÇ…Ç»Ç¡ÇƒÇΩÇÃÇ≈Ç±Ç±Ç≈Ç–Ç∆Ç‹Ç∏èCê≥
    WorldNormal=normalize((MMatrix*vec4(-normal,0.0)).xyz);
    v2f_id = float(gl_InstanceID);
}

)"