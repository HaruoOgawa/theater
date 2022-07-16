R"(

#version 410

layout(triangles, equal_spacing, ccw) in;

//out vec3 t2f_normal;
uniform float _time;

//uniform sampler2D _heightMap;

in vec3 tesc2tese_normal[];

out float pid;
out vec3 t2f_normal;
out vec2 e2g_uv;

void main(){
	float u=gl_TessCoord.x;
	float v=gl_TessCoord.y;

	vec4 v0=gl_in[0].gl_Position;
	vec4 v1=gl_in[1].gl_Position;
	vec4 v2=gl_in[2].gl_Position;
	
    vec3 normal=normalize(
         tesc2tese_normal[0]*gl_TessCoord.x +
         tesc2tese_normal[1]*gl_TessCoord.y + 
         tesc2tese_normal[2]*gl_TessCoord.z 
    );
    t2f_normal=normal;

    //height
    //vec3 height=texture(_heightMap,gl_TessCoord.xy).rgb;
    vec3 height=vec3(sin( dot(vec2(0.0,1.0),gl_TessCoord.xy)*100.0+_time )*0.1);

	vec4 pos=vec4(v0.xyz*gl_TessCoord.x+v1.xyz*gl_TessCoord.y+v2.xyz*gl_TessCoord.z,1.0); 
    pos.xyz+=(1.0-height.r)*1000.0*normal;

    gl_Position=pos;
	pid=float(gl_PrimitiveID);
    e2g_uv=gl_TessCoord.xy;

}

)"