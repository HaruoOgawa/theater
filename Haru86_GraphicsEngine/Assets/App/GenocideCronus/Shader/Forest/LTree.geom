R"(

#version 430

layout(lines) in;
layout(line_strip,2) out;

uniform mat4 MVPMatrix;
uniform mat4 MMatrix;
uniform mat4 VMatrix;
uniform mat4 PMatrix;
uniform float _time;
uniform float _deltaTime;
uniform vec3 _CameraPos;

void main()
{
	gl_Position = MVPMatrix * gl_in[0].gl_Position;
	EmitVertex;
	
	gl_Position = MVPMatrix * gl_in[1].gl_Position;
	EmitVertex;

	EndPrimitive();
}

)"