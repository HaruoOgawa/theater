R"(

#version 330

uniform samplerCube _WaterRP;

in vec2 uv;
in vec3 WorldNormal;
in vec3 WorldVertexPos;
in vec3 CameraPos;

void main(){
	vec3 col=vec3(0.0);
	
	//
	vec3 ref=reflect(-normalize(WorldVertexPos-CameraPos),WorldNormal);
	col=texture(_WaterRP,ref).rgb;
	//col=vec3(uv,0.0);
	gl_FragColor=vec4(col,1.0);
}

)"