#version 430

layout(triangles) in;
layout(triangle_strip,max_vertices=3) out;

in vec2 uv[];
in vec3 randCol[];
in vec3 v2g_normal[];
in float v2g_flag[];

out vec2 g2f_uv;
out vec3 g2f_randCol;
out vec3 g2f_normal;
out float g2f_flag;

uniform float _time;

void main(){
	gl_Position=gl_in[0].gl_Position;
	g2f_uv=uv[0];
	g2f_randCol=randCol[0];
	g2f_normal=v2g_normal[0];
	g2f_flag=v2g_flag[0];
	EmitVertex();
	
	gl_Position=gl_in[1].gl_Position;
	g2f_uv=uv[1];
	g2f_randCol=randCol[1];
	g2f_normal=v2g_normal[1];
	g2f_flag=v2g_flag[1];
	EmitVertex();
	
	gl_Position=gl_in[2].gl_Position;
	g2f_uv=uv[2];
	g2f_randCol=randCol[2];
	g2f_normal=v2g_normal[2];
	g2f_flag=v2g_flag[2];
	EmitVertex();

	EndPrimitive();
}
