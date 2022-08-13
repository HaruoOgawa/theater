R"(

#version 410

layout(vertices=3)out;

in vec3 v2tesc_WorldNormal[];

out vec3 tesc2tese_normal[];

uniform vec3 _WorldCameraPos;

void main(){
	//
	float outLevel=32.0; //32.0;
	float inLevel=32.0; //16.0;

	/*if( length(gl_in[gl_InvocationID].gl_Position.xyz-_WorldCameraPos) < 2.5 )
	{
		outLevel=16.0;
		inLevel=8.0;
	}*/

	//
	gl_out[gl_InvocationID].gl_Position=gl_in[gl_InvocationID].gl_Position;	
	
	tesc2tese_normal[gl_InvocationID]=v2tesc_WorldNormal[gl_InvocationID];
	
	gl_TessLevelOuter[0]=outLevel;
	gl_TessLevelOuter[1]=outLevel;
	gl_TessLevelOuter[2]=outLevel;
	
	gl_TessLevelInner[0]=inLevel;
	
}

)"