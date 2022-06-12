#version 430

layout(points) in;
layout(triangle_strip,max_vertices=32) out;

struct v2g{
    vec3 node_pos ;
    vec3 node_nextPos;
    vec3 node_dir;
    vec3 node_nextDir;
    vec3 node_previousDir;
    float node_life;
    float trail_ID ;
    int nowIndexInNodes;
    vec4 trail_color;
};

struct g2f{
	vec4 vertex ;
	vec2 uv ;
	float node_life;
	float trail_ID ;
	vec4 trail_color;
    vec3 normal;
    vec3 worldPosition;
};

in float v2g_flag[];
in vec3 v2g_pos[];
in v2g v2g_o[];

out float g2f_flag;
out g2f g2f_o;

uniform mat4 MVPMatrix;
uniform mat4 MMatrix;
uniform mat4 VMatrix;
uniform mat4 PMatrix;
uniform float _time;
uniform float _deltaTime;
uniform vec3 _WorldSpaceCameraPos;
uniform float _initNodeLife;
uniform float _nodeSegment;
uniform float _TrailWidth;
uniform int _boids_count;

#define PI 3.14159265
#define rot(a) mat2(cos(a),sin(a),-sin(a),cos(a))

float rand(vec2 seeds){
  return fract(sin(dot(seeds, vec2(12.9898,78.233))) * 43758.5453);
}

void main(){

    vec3 Tangent=normalize(v2g_o[0].node_dir);
    vec3 Normal=normalize(cross(Tangent,v2g_o[0].node_previousDir));
    vec3 BioNormal=normalize(cross(Tangent,Normal));
    vec3 Pos=v2g_o[0].node_pos;
    
    vec3 NextTangent=normalize(v2g_o[0].node_nextDir);
    vec3 NextNormal=normalize(cross(NextTangent,Tangent));
    vec3 NextBioNormal=normalize(cross(NextTangent,NextNormal));
    vec3 NextPos=v2g_o[0].node_nextPos;

    float nowIndexInNodes=float(v2g_o[0].nowIndexInNodes);
    float nextUVx=(nowIndexInNodes+1)/_nodeSegment;
    int N=8;

    //Create Surface
     for(int i=0;i<N;i++){
        //float a=(PI/2.0)*float(i);
        //float b=(PI/2.0)*float(i+1);
        
        float a=((2.0*PI)/float(N))*float(i);
        float b=((2.0*PI)/float(N))*float(i+1);
     
        //v00
	    gl_Position=MVPMatrix*vec4(Pos+normalize(cos(a)*Normal+sin(a)*BioNormal)*_TrailWidth,1.0);
        g2f_o.uv=vec2(nowIndexInNodes/_nodeSegment,1.0);
        g2f_o.uv.x*=_nodeSegment;
        g2f_o.trail_ID=v2g_o[0].trail_ID;
        g2f_o.node_life=v2g_o[0].node_life;
        g2f_o.trail_color=v2g_o[0].trail_color;
        g2f_o.normal=(MMatrix*vec4(Normal,1.0)).xyz;
        g2f_o.worldPosition=(MMatrix*vec4(Pos+normalize(cos(a)*Normal+sin(a)*BioNormal)*_TrailWidth,1.0)).xyz;
        EmitVertex();

        //v01
        gl_Position=MVPMatrix*vec4(NextPos-normalize(cos(a)*NextNormal+sin(a)*NextBioNormal)*_TrailWidth,1.0);
        g2f_o.uv=vec2(
         (nextUVx>1.0) ? 0.0 : nextUVx
        ,1.0);
        g2f_o.uv.x*=_nodeSegment;
        g2f_o.trail_ID=v2g_o[0].trail_ID;
        g2f_o.node_life=v2g_o[0].node_life;
        g2f_o.trail_color=v2g_o[0].trail_color;
        g2f_o.normal=(MMatrix*vec4(NextNormal,1.0)).xyz;
        g2f_o.worldPosition=(MMatrix*vec4(NextPos-normalize(cos(a)*NextNormal+sin(a)*NextBioNormal)*_TrailWidth,1.0)).xyz;
        EmitVertex();

        //v02
        gl_Position=MVPMatrix*vec4(Pos+normalize(cos(b)*Normal+sin(b)*BioNormal)*_TrailWidth,1.0);
        g2f_o.uv=vec2(nowIndexInNodes/_nodeSegment,0.0);
        g2f_o.uv.x*=_nodeSegment;
        g2f_o.trail_ID=v2g_o[0].trail_ID;
        g2f_o.node_life=v2g_o[0].node_life;
        g2f_o.trail_color=v2g_o[0].trail_color;
        g2f_o.normal=(MMatrix*vec4(-Normal,1.0)).xyz;
        g2f_o.worldPosition=(MMatrix*vec4(Pos+normalize(cos(b)*Normal+sin(b)*BioNormal)*_TrailWidth,1.0)).xyz;
        EmitVertex();
            
        //v03
        gl_Position=MVPMatrix*vec4(NextPos-normalize(cos(b)*NextNormal+sin(b)*NextBioNormal)*_TrailWidth,1.0);
        g2f_o.uv=vec2(
         (nextUVx>1.0) ? 0.0 : nextUVx
        ,0.0);
        g2f_o.uv.x*=_nodeSegment;
        g2f_o.trail_ID=v2g_o[0].trail_ID;
        g2f_o.node_life=v2g_o[0].node_life;
        g2f_o.trail_color=v2g_o[0].trail_color;
        g2f_o.normal=(MMatrix*vec4(-NextNormal,1.0)).xyz;
        g2f_o.worldPosition=(MMatrix*vec4(NextPos-normalize(cos(b)*NextNormal+sin(b)*NextBioNormal)*_TrailWidth,1.0)).xyz;
        EmitVertex();

        EndPrimitive();
     }

}
