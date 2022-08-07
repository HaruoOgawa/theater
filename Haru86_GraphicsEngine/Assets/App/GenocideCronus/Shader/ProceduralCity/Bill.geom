R"(

#version 410

layout(triangles) in;
layout(triangle_strip,max_vertices=10) out;

uniform mat4 MVPMatrix;
uniform mat4 MMatrix;
uniform mat4 VMatrix;
uniform mat4 PMatrix;
uniform float _time;
uniform float _deltaTime;
uniform vec3 _CameraPos;

in float t2g_id[];
in vec3 t2g_normal[];
in vec4 t2g_billinfo[];
in vec3 t2g_WorldVertexPos[];

out float g2f_id;
out vec3 g2f_normal;
out vec4 g2f_billinfo;
out vec3 g2f_WorldVertexPos;

void Createvertex(vec3 offset)
{
	gl_Position = MVPMatrix * (gl_in[0].gl_Position + vec4(offset,0.0));
	g2f_id=t2g_id[0];
	g2f_WorldVertexPos = (MMatrix* gl_in[0].gl_Position).xyz;
	g2f_normal = t2g_normal[0];
	g2f_billinfo = t2g_billinfo[0];

	EmitVertex();

	gl_Position = MVPMatrix * (gl_in[1].gl_Position + vec4(offset,0.0));
	g2f_id=t2g_id[1];
	g2f_WorldVertexPos = (MMatrix* gl_in[1].gl_Position).xyz;
	g2f_normal = t2g_normal[1];
	g2f_billinfo = t2g_billinfo[1];

	EmitVertex();

	gl_Position = MVPMatrix * (gl_in[2].gl_Position + vec4(offset,0.0));
	g2f_id=t2g_id[2];
	g2f_WorldVertexPos = (MMatrix* gl_in[2].gl_Position).xyz;
	g2f_normal = t2g_normal[2];
	g2f_billinfo = t2g_billinfo[2];

	EmitVertex();

	EndPrimitive();
}

void main()
{
	Createvertex(vec3(0.0));
}

)"