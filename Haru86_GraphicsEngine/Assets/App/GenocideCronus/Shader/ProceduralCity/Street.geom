R"(

#version 410

layout(triangles) in;
layout(triangle_strip,max_vertices =12) out;

//in vec2 v2g_uv[];
in vec3 tese2g_WorldNormal[];
flat in int tese2g_id[];

//out vec2 uv;
out vec3 WorldVertexPos;
out vec3 WorldNormal;
flat out int PrimID;

uniform mat4 MVPMatrix;
uniform mat4 MMatrix;
uniform mat4 VMatrix;
uniform mat4 PMatrix;
uniform float _time;
uniform float _deltaTime;
uniform vec3 _WorldCameraPos;

void main()
{
	gl_Position = MVPMatrix * gl_in[0].gl_Position;
	WorldVertexPos = (MMatrix*gl_in[0].gl_Position).xyz;
	WorldNormal = normalize(MMatrix*vec4(tese2g_WorldNormal[0],0.0)).xyz;
	//uv=v2g_uv[0];
	PrimID=tese2g_id[0];
	EmitVertex();

	gl_Position = MVPMatrix * gl_in[1].gl_Position;
	WorldVertexPos = (MMatrix*gl_in[1].gl_Position).xyz;
	WorldNormal = normalize(MMatrix*vec4(tese2g_WorldNormal[1],0.0)).xyz;
	//uv=v2g_uv[1];
	PrimID=tese2g_id[1];
	EmitVertex();

	gl_Position = MVPMatrix * gl_in[2].gl_Position;
	WorldVertexPos = (MMatrix*gl_in[2].gl_Position).xyz;
	WorldNormal = normalize(MMatrix*vec4(tese2g_WorldNormal[2],0.0)).xyz;
	//uv=v2g_uv[2];
	PrimID=tese2g_id[2];
	EmitVertex();

	EndPrimitive();
}

)"