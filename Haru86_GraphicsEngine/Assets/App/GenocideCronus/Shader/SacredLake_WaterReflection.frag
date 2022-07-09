R"(

#version 330

uniform samplerCube _WaterRP;
uniform sampler2D _MonoDirRP;

in vec2 uv;
in vec3 WorldNormal;
in vec3 WorldVertexPos;
in vec3 CameraPos;

void main(){
	vec3 col=vec3(0.0);
	
	//
	//vec3 ref=reflect(-normalize(WorldVertexPos-CameraPos),WorldNormal);
	//col=texture(_WaterRP,ref).rgb;
	
	//vec2 st=vec2(uv.x,1.0-uv.y);
	//col=texture(_MonoDirRP,st).rgb;
	col=vec3(uv,0.0);

	gl_FragColor=vec4(col,1.0);
}

)"