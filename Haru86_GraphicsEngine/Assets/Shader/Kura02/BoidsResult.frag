#version 430

in vec2 g2f_uv;
in vec3 g2f_randCol;
in vec3 g2f_normal;
in float g2f_flag;

#define LightDir vec3(-0.613121, -0.1916, 0.766402)

void main(){
	vec4 col=vec4(1.0);
	col.rgb=vec3(0.0);
	gl_FragColor=col;
}
