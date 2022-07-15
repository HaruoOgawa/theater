R"(

#version 410

layout(triangles, equal_spacing, ccw) in;

//out vec3 t2f_normal;
uniform float _time;

void main()
{
	vec3 v0=gl_in[0].gl_Position.xyz;
	vec3 v1=gl_in[1].gl_Position.xyz;
	vec3 v2=gl_in[2].gl_Position.xyz;

	/*vec3 normal = normalize(
		cross( (v1-v0) , (v2-v0) ) +
		cross( (v0-v1) , (v2-v1) ) +
		cross( (v0-v2) , (v1-v2) )
	);

	t2f_normal=normal;*/

	vec4 pos=vec4(v0.xyz*gl_TessCoord.x+v1.xyz*gl_TessCoord.y+v2.xyz*gl_TessCoord.z,1.0); 
	//pos.z+= 0.1 * sin( dot(vec2(0.0,1.0),pos.xy)*440.0 +_time );

	gl_Position=pos;
}

)"