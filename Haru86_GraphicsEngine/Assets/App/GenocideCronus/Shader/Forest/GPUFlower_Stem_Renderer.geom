R"(

#version 430

layout(points) in;
layout(triangle_strip,max_vertices=72) out;

uniform mat4 MVPMatrix;
uniform mat4 MMatrix;
uniform mat4 VMatrix;
uniform mat4 PMatrix;
uniform float _time;
uniform float _deltaTime;
uniform vec3 _CameraPos;

#define PI 3.14159265

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
    int resampleGroupIndex;
    vec4 position;
    vec4 tangent;
    vec4 normal;
    vec4 bioNormal;
    int renderFlag;
    float lifeTime;
};

// 各花の生える基本の座標
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

struct g2f
{
    vec2 uv;
    //vec4 vertex;
    vec3 normal;
    vec3 worldPos;
};

uniform int _stemVertexCount;
uniform int _stemSegments;
uniform float _stemRadius;

in v2g v2g_o[];
out g2f g2f_o;

out vec2 uv;
out vec3 CameraPos;
out vec3 WorldVertexPos;
out vec3 WorldNormal;

void main(){
    float lifeTime=v2g_o[0].lifeTime;

    if(v2g_o[0].idInMyStem>0&&v2g_o[0].idInMyStem<_stemVertexCount-1){
        float angleVal=(2.0*PI)/_stemSegments;

        for(int i=0;i<_stemSegments;i++){
                        
            vec4 pos0=vec4(
                lifeTime*_stemRadius*normalize(v2g_o[0].normal*cos(angleVal*float(i))
                +v2g_o[0].bioNormal*sin(angleVal*float(i)))
                +gl_in[0].gl_Position.xyz
                ,1.0);

            vec4 pos1=vec4(
                lifeTime*_stemRadius*normalize(v2g_o[0].normal*cos(angleVal*float(i+1))
                +v2g_o[0].bioNormal*sin(angleVal*float(i+1)))
                +gl_in[0].gl_Position.xyz
                ,1.0);

            vec4 pos2=vec4(
                lifeTime*_stemRadius*normalize(v2g_o[0].nextNormal*cos(angleVal*float(i))
                +v2g_o[0].nextBioNormal*sin(angleVal*float(i)))
                +v2g_o[0].nextStemVertex.xyz
                ,1.0);

            vec4 pos3=vec4(
                lifeTime*_stemRadius*normalize(v2g_o[0].nextNormal*cos(angleVal*float(i+1))
                +v2g_o[0].nextBioNormal*sin(angleVal*float(i+1)))
                +v2g_o[0].nextStemVertex.xyz
                ,1.0);
                        
            //first
            gl_Position=MVPMatrix*(pos0);
            g2f_o.uv=vec2(0,0);
            g2f_o.normal=normalize(pos0.xyz-gl_in[0].gl_Position.xyz);
            g2f_o.worldPos=(MMatrix*pos0).xyz;
            uv =vec2(0,0);
            CameraPos =_CameraPos;
            WorldVertexPos = (MMatrix * pos0).xyz;
            WorldNormal = (MMatrix * vec4(normalize(g2f_o.normal),0.0)).xyz;
            EmitVertex();

            gl_Position=MVPMatrix*(pos1);
            g2f_o.uv=vec2(0,0);
            g2f_o.normal=normalize(pos1.xyz-gl_in[0].gl_Position.xyz);
            g2f_o.worldPos=(MMatrix*pos1).xyz;
            uv =vec2(0,0);
            CameraPos =_CameraPos;
            WorldVertexPos = (MMatrix * pos1).xyz;
            WorldNormal = (MMatrix * vec4(normalize(g2f_o.normal),0.0)).xyz;
            EmitVertex();  

            gl_Position=MVPMatrix*(pos3);
            g2f_o.uv=vec2(0,0);
            g2f_o.normal=normalize(pos3.xyz-v2g_o[0].nextStemVertex.xyz);
            g2f_o.worldPos=(MMatrix*pos3).xyz;
            uv =vec2(0,0);
            CameraPos =_CameraPos;
            WorldVertexPos = (MMatrix * pos3).xyz;
            WorldNormal = (MMatrix * vec4(normalize(g2f_o.normal),0.0)).xyz;
            EmitVertex();    

            EndPrimitive();

            //second
            gl_Position=MVPMatrix*(pos0);
            g2f_o.uv=vec2(0,0);
            g2f_o.normal=normalize(pos0.xyz-gl_in[0].gl_Position.xyz);
            g2f_o.worldPos=(MMatrix*pos0).xyz;
            uv =vec2(0,0);
            CameraPos =_CameraPos;
            WorldVertexPos = (MMatrix * pos0).xyz;
            WorldNormal = (MMatrix * vec4(normalize(g2f_o.normal),0.0)).xyz;
            EmitVertex();

            gl_Position=MVPMatrix*(pos3);
            g2f_o.uv=vec2(0,0);
            g2f_o.normal=normalize(pos3.xyz-v2g_o[0].nextStemVertex.xyz);
            g2f_o.worldPos=(MMatrix*pos3).xyz;
            uv =vec2(0,0);
            CameraPos =_CameraPos;
            WorldVertexPos = (MMatrix * pos3).xyz;
            WorldNormal = (MMatrix * vec4(normalize(g2f_o.normal),0.0)).xyz;
            EmitVertex();  

            gl_Position=MVPMatrix*(pos2);
            g2f_o.uv=vec2(0,0);
            g2f_o.normal=normalize(pos2.xyz-v2g_o[0].nextStemVertex.xyz);
            g2f_o.worldPos=(MMatrix*pos2).xyz;
            uv =vec2(0,0);
            CameraPos =_CameraPos;
            WorldVertexPos = (MMatrix * pos2).xyz;
            WorldNormal = (MMatrix * vec4(normalize(g2f_o.normal),0.0)).xyz;
            EmitVertex();    

            EndPrimitive();
        }

    }else{
        g2f o;

        gl_Position=gl_in[0].gl_Position;
        g2f_o.uv=vec2(0,0);
        g2f_o.normal=normalize(vec3(0,0,1));
        g2f_o.worldPos=(MMatrix*gl_in[0].gl_Position).xyz;
                    uv =vec2(0,0);
            CameraPos =_CameraPos;
            WorldVertexPos = (MMatrix * gl_in[0].gl_Position).xyz;
            WorldNormal = (MMatrix * vec4(normalize(vec3(0,0,1)),0.0)).xyz;
        EmitVertex();

        gl_Position=gl_in[0].gl_Position;
        g2f_o.uv=vec2(0,0);
        g2f_o.normal=normalize(vec3(0,0,1));
        g2f_o.worldPos=(MMatrix*gl_in[0].gl_Position).xyz;
                    uv =vec2(0,0);
            CameraPos =_CameraPos;
            WorldVertexPos = (MMatrix * gl_in[0].gl_Position).xyz;
            WorldNormal = (MMatrix * vec4(normalize(vec3(0,0,1)),0.0)).xyz;
        EmitVertex();

        gl_Position=gl_in[0].gl_Position;
        g2f_o.uv=vec2(0,0);
        g2f_o.normal=normalize(vec3(0,0,1));
        g2f_o.worldPos=(MMatrix*gl_in[0].gl_Position).xyz;
                    uv =vec2(0,0);
            CameraPos =_CameraPos;
            WorldVertexPos = (MMatrix * gl_in[0].gl_Position).xyz;
            WorldNormal = (MMatrix * vec4(normalize(vec3(0,0,1)),0.0)).xyz;
        EmitVertex();

        EndPrimitive();

            gl_Position=gl_in[0].gl_Position;
        g2f_o.uv=vec2(0,0);
        g2f_o.normal=normalize(vec3(0,0,1));
        g2f_o.worldPos=(MMatrix*gl_in[0].gl_Position).xyz;
                    uv =vec2(0,0);
            CameraPos =_CameraPos;
            WorldVertexPos = (MMatrix * gl_in[0].gl_Position).xyz;
            WorldNormal = (MMatrix * vec4(normalize(vec3(0,0,1)),0.0)).xyz;
        EmitVertex();

        gl_Position=gl_in[0].gl_Position;
        g2f_o.uv=vec2(0,0);
        g2f_o.normal=normalize(vec3(0,0,1));
        g2f_o.worldPos=(MMatrix*gl_in[0].gl_Position).xyz;
                    uv =vec2(0,0);
            CameraPos =_CameraPos;
            WorldVertexPos = (MMatrix * gl_in[0].gl_Position).xyz;
            WorldNormal = (MMatrix * vec4(normalize(vec3(0,0,1)),0.0)).xyz;
        EmitVertex();

        gl_Position=gl_in[0].gl_Position;
        g2f_o.uv=vec2(0,0);
        g2f_o.normal=normalize(vec3(0,0,1));
        g2f_o.worldPos=(MMatrix*gl_in[0].gl_Position).xyz;
                    uv =vec2(0,0);
            CameraPos =_CameraPos;
            WorldVertexPos = (MMatrix * gl_in[0].gl_Position).xyz;
            WorldNormal = (MMatrix * vec4(normalize(vec3(0,0,1)),0.0)).xyz;
        EmitVertex();

        EndPrimitive();
    }
}

)"