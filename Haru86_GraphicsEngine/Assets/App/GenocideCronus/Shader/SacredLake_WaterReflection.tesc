R"(

#version 410

//layout(vertices=4)out;
layout(vertices=3)out;

in vec3 v2tesc_normal[];
out vec3 tesc2tese_normal[];

float outLevel=512.0;
float inLevel=246.0;

void main(){
	gl_out[gl_InvocationID].gl_Position=gl_in[gl_InvocationID].gl_Position;	
	tesc2tese_normal[gl_InvocationID]=v2tesc_normal[gl_InvocationID];

	gl_TessLevelOuter[0]=outLevel;
	gl_TessLevelOuter[1]=outLevel;
	gl_TessLevelOuter[2]=outLevel;
	
	gl_TessLevelInner[0]=inLevel;
	
}

)"