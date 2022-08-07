R"(

#version 410

layout(vertices=3)out;

in vec3 v2tesc_normal[];
in vec4 v2tesc_billinfo[];

out vec3 tesc2tese_normal[];
out vec4 tesc2tese_billinfo[];

float outLevel=4.0;
float inLevel=2.0;

void main(){
	gl_out[gl_InvocationID].gl_Position=gl_in[gl_InvocationID].gl_Position;	
	
	tesc2tese_normal[gl_InvocationID]=v2tesc_normal[gl_InvocationID];
	tesc2tese_billinfo[gl_InvocationID]=v2tesc_billinfo[gl_InvocationID];

	gl_TessLevelOuter[0]=outLevel;
	gl_TessLevelOuter[1]=outLevel;
	gl_TessLevelOuter[2]=outLevel;
	
	gl_TessLevelInner[0]=inLevel;
	
}

)"