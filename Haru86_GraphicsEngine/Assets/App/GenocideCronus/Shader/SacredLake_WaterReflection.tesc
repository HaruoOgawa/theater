R"(

#version 410

layout(vertices=3)out;

float outLevel=16.0;
float inLevel=8.0;

void main()
{
	gl_out[gl_InvocationID].gl_Position=gl_in[gl_InvocationID].gl_Position;	
	
	gl_TessLevelOuter[0]=outLevel;
	gl_TessLevelOuter[1]=outLevel;
	gl_TessLevelOuter[2]=outLevel;
	
	gl_TessLevelInner[0]=inLevel;
}

)"