R"(

#version 410

layout(vertices=3)out;

in vec3 v2tesc_normal[];
in vec4 v2tesc_billinfo[];
in float v2tesc_id[];

out vec3 tesc2tese_normal[];
out vec4 tesc2tese_billinfo[];
out float tesc2tese_id[];

uniform vec3 _WorldCameraPos;

void main(){
	//
	float outLevel=1.0; //32.0;
	float inLevel=1.0; //16.0;

	if( length(gl_in[gl_InvocationID].gl_Position.xyz-_WorldCameraPos) < 2.5 )
	{
		outLevel=16.0;
		inLevel=8.0;
	}

	//
	gl_out[gl_InvocationID].gl_Position=gl_in[gl_InvocationID].gl_Position;	
	
	tesc2tese_normal[gl_InvocationID]=v2tesc_normal[gl_InvocationID];
	tesc2tese_billinfo[gl_InvocationID]=v2tesc_billinfo[gl_InvocationID];
	tesc2tese_id[gl_InvocationID]=v2tesc_id[gl_InvocationID];

	gl_TessLevelOuter[0]=outLevel;
	gl_TessLevelOuter[1]=outLevel;
	gl_TessLevelOuter[2]=outLevel;
	
	gl_TessLevelInner[0]=inLevel;
	
}

)"