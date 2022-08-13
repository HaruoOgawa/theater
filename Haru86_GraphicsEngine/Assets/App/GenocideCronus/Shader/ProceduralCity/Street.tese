R"(

#version 410

layout(triangles, equal_spacing, ccw) in;

in vec3 tesc2tese_normal[];

out vec3 tese2g_WorldNormal;
flat out int tese2g_id;

uniform float _time;

 float rand(vec2 st)
{
    return fract(sin(dot(st, vec2(12.9898, 78.233))) * 43758.5453);
}

void main(){
    //
	vec4 v0=gl_in[0].gl_Position;
	vec4 v1=gl_in[1].gl_Position;
	vec4 v2=gl_in[2].gl_Position;
	
    //
    vec3 normal=normalize(
         tesc2tese_normal[0]*gl_TessCoord.x +
         tesc2tese_normal[1]*gl_TessCoord.y + 
         tesc2tese_normal[2]*gl_TessCoord.z 
    );
    tese2g_WorldNormal=normal.xyz;
    // なぜかテッセレーションシェーダーを通すと法線が裏返るのでここで反転させる
    //tese2g_WorldNormal=-normal.xyz;

    //
	vec4 pos=vec4(v0.xyz*gl_TessCoord.x+v1.xyz*gl_TessCoord.y+v2.xyz*gl_TessCoord.z,1.0); 
    //pos.y+=sin(pos.x*100.0+_time)*0.01;

    // Output
    gl_Position=pos;
    tese2g_id=int(1000.0*rand(vec2(gl_TessCoord.x+gl_TessCoord.z)));
}

)"