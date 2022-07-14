R"(

#version 410

layout(location=0)in vec3 vertex;
layout(location=1)in vec3 normal;
layout(location=2)in vec2 texcoord;
layout(location=3)in vec4 weights;
layout(location=4)in ivec4 joints;

void main()
{
	vec4 pos=vec4(vertex,1.0);
	gl_Position=pos;
}

)"