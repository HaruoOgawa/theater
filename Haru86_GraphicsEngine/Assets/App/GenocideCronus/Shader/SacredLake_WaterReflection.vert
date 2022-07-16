R"(

#version 410

layout(location=0)in vec3 vertex;
layout(location=1)in vec3 normal;
layout(location=2)in vec2 texcoord;
layout(location=3)in vec4 weights;
layout(location=4)in ivec4 joints;

out vec3 v2tesc_normal;

#define rot(a) mat2(cos(a),-sin(a),sin(a),cos(a))
#define PI 3.14159265

void main(){
	vec4 pos=vec4(vertex,1.0);
	//pos.xz*=rot(_time*0.001);
	//pos.xy*=rot(_time*0.001);
	//pos.yz*=rot(_time*0.001);
	
	//pos.yz*=rot(PI/6.0);
	
	gl_Position=pos;
	v2tesc_normal=(vec4(normal,1.0)).xyz;
}

)"