R"(

#version 430

layout(points) in;
layout(triangle_strip,max_vertices=256) out;

uniform mat4 MVPMatrix;
uniform mat4 MMatrix;
uniform mat4 VMatrix;
uniform mat4 PMatrix;

struct v2g{
	vec4 position;
	vec2 uv;
	vec3 worldPos;
};

struct g2f{
	vec2 uv;
};

in v2g v2g_o[]; 

out g2f g2f_o; 

void main()
{
	gl_Position=MVPMatrix*vec4(vec3(gl_in[0].gl_Position.xyz+normalize(vec3(-0.5,-0.5,0.0))),1.0);
	g2f_o.uv=vec2(0.0,0.0);
	EmitVertex();

	gl_Position=MVPMatrix*vec4(vec3(gl_in[0].gl_Position.xyz+normalize(vec3(-0.5,0.5,0.0))),1.0);
	g2f_o.uv=vec2(0.0,1.0);
	EmitVertex();

	gl_Position=MVPMatrix*vec4(vec3(gl_in[0].gl_Position.xyz+normalize(vec3(0.5,0.5,0.0))),1.0);
	g2f_o.uv=vec2(1.0,1.0);
	EmitVertex();
	
	EndPrimitive();

	gl_Position=MVPMatrix*vec4(vec3(gl_in[0].gl_Position.xyz+normalize(vec3(0.5,0.5,0.0))),1.0);
	g2f_o.uv=vec2(1.0,1.0);
	EmitVertex();

	gl_Position=MVPMatrix*vec4(vec3(gl_in[0].gl_Position.xyz+normalize(vec3(0.5,-0.5,0.0))),1.0);
	g2f_o.uv=vec2(1.0,0.0);
	EmitVertex();

	gl_Position=MVPMatrix*vec4(vec3(gl_in[0].gl_Position.xyz+normalize(vec3(-0.5,-0.5,0.0))),1.0);
	g2f_o.uv=vec2(0.0,0.0);
	EmitVertex();
	
	EndPrimitive();
	
}

)"