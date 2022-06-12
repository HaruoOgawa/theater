#version 330

in vec2 uv;

out vec4 outColor;

void main(){
	vec2 st=uv*2.0-1.0;
	outColor=vec4(vec3(mod(length(st),0.1)*10.0),1.0);
}