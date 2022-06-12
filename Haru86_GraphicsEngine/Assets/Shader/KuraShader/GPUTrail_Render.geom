#version 430

layout(points) in;
layout(triangle_strip,max_vertices=4) out;

struct v2g{
    vec3 node_pos ;
    vec3 node_nextPos;
    vec3 node_dir;
    vec3 node_nextDir;
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

   vec3 camDir=normalize(v2g_o[0].node_pos-_WorldSpaceCameraPos.xyz);
    vec3 camNextDir=normalize(v2g_o[0].node_nextPos-_WorldSpaceCameraPos.xyz);
    vec3 node_cam_sideDir=normalize(cross(camDir,v2g_o[0].node_dir));
    vec3 node_cam_nextSideDir=normalize(cross(camNextDir,v2g_o[0].node_nextDir));

    vec4 render_node_pos01=vec4(v2g_o[0].node_pos+node_cam_sideDir*_TrailWidth,1.0);
    vec4 render_node_pos0_1=vec4(v2g_o[0].node_pos-node_cam_sideDir*_TrailWidth,1.0);
    vec4 render_node_pos11=vec4(v2g_o[0].node_nextPos+node_cam_nextSideDir*_TrailWidth,1.0);
    vec4 render_node_pos1_1=vec4(v2g_o[0].node_nextPos-node_cam_nextSideDir*_TrailWidth,1.0);

    float nowIndexInNodes=float(v2g_o[0].nowIndexInNodes);

	//Create Surface
	gl_Position=MVPMatrix*render_node_pos01;
    g2f_o.uv=vec2(nowIndexInNodes/_nodeSegment,1.0);
    g2f_o.uv.x*=_nodeSegment;
    g2f_o.trail_ID=v2g_o[0].trail_ID;
    g2f_o.node_life=v2g_o[0].node_life;
    g2f_o.trail_color=v2g_o[0].trail_color;
    EmitVertex();

    gl_Position=MVPMatrix*render_node_pos0_1;
    g2f_o.uv=vec2(nowIndexInNodes/_nodeSegment,0.0);
    g2f_o.uv.x*=_nodeSegment;
    g2f_o.trail_ID=v2g_o[0].trail_ID;
    g2f_o.node_life=v2g_o[0].node_life;
    g2f_o.trail_color=v2g_o[0].trail_color;
    EmitVertex();

    float nextUVx=(nowIndexInNodes+1)/_nodeSegment;
                
    gl_Position=MVPMatrix*render_node_pos11;
    g2f_o.uv=vec2(
     (nextUVx>1.0) ? 0.0 : nextUVx
    ,1.0);
    g2f_o.uv.x*=_nodeSegment;
    g2f_o.trail_ID=v2g_o[0].trail_ID;
    g2f_o.node_life=v2g_o[0].node_life;
    g2f_o.trail_color=v2g_o[0].trail_color;
    EmitVertex();

    gl_Position=MVPMatrix*render_node_pos1_1;
    g2f_o.uv=vec2(
     (nextUVx>1.0) ? 0.0 : nextUVx
    ,0.0);
    g2f_o.uv.x*=_nodeSegment;
    g2f_o.trail_ID=v2g_o[0].trail_ID;
    g2f_o.node_life=v2g_o[0].node_life;
    g2f_o.trail_color=v2g_o[0].trail_color;
    EmitVertex();

    EndPrimitive();


}
