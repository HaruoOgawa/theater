R"(

#version 410

layout(triangles, equal_spacing, ccw) in;

in vec3 tesc2tese_normal[];
in vec4 tesc2tese_billinfo[];

out float t2g_pid;
out vec3 t2g_normal;
out vec4 t2g_billinfo;
out vec3 t2g_WorldVertexPos;

void main(){
	//float u=gl_TessCoord.x;
	//float v=gl_TessCoord.y;

	vec4 v0=gl_in[0].gl_Position;
	vec4 v1=gl_in[1].gl_Position;
	vec4 v2=gl_in[2].gl_Position;
	
    vec3 normal=normalize(
         tesc2tese_normal[0]*gl_TessCoord.x +
         tesc2tese_normal[1]*gl_TessCoord.y + 
         tesc2tese_normal[2]*gl_TessCoord.z 
    );
    t2g_normal=normal.xyz;

	vec4 pos=vec4(v0.xyz*gl_TessCoord.x+v1.xyz*gl_TessCoord.y+v2.xyz*gl_TessCoord.z,1.0); 
    
    //vec3 height=vec3(sin( dot(vec2(0.0,1.0),gl_TessCoord.xy)*100.0+_time )*0.1);
    //pos.xyz+=(1.0-height.r)*1000.0*normal;

    // Output
    gl_Position=pos;
	t2g_pid=float(gl_PrimitiveID);
    t2g_billinfo=tesc2tese_billinfo[0];
    t2g_WorldVertexPos=pos.xyz;
}

)"