R"(

#version 330

uniform samplerCube _WaterRP;
//uniform sampler2D _WaterRP;

in vec2 uv;
in vec3 WorldNormal;
in vec3 WorldVertexPos;
in vec3 CameraPos;

void main(){
	vec3 col=vec3(0.0);
	
	//
	vec3 ref=reflect(WorldVertexPos-CameraPos,WorldNormal);
	col=texture(_WaterRP,ref).rgb;
	
	gl_FragColor=vec4(col,1.0);
}

)"