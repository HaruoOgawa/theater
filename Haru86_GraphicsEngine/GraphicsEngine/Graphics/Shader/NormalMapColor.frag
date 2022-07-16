R"(

#version 330

in vec2 uv;
in vec3 WorldNormal;

uniform int _NotUseNormal;

void main(){
	vec3 col=vec3(0.0);
	if(_NotUseNormal != 1)col=WorldNormal*0.5+0.5;
	gl_FragColor=vec4(col,1.0);
}

)"