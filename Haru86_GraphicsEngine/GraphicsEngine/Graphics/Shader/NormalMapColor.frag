R"(

#version 330

in vec2 uv;
in vec3 WorldNormal;

uniform int _NotUseNormal;

void main(){
	vec4 col=vec4(0.0);
	if(_NotUseNormal != 1)col=vec4(WorldNormal*0.5+0.5,1.0);
	gl_FragColor=col;
}

)"