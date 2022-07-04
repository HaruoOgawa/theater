R"(

#version 330

uniform samplerCube _WaterRP;
//uniform sampler2D _WaterRP;

in vec2 uv;
in vec3 WorldNormal;

void main(){
	vec3 col=vec3(0.0);
	//col=texture(_WaterRP,uv).rgb;
	//col=vec3(uv,0.0);
	col=texture(_WaterRP,WorldNormal).rgb;
	//col=WorldNormal*2.0+1.0;
	gl_FragColor=vec4(col,1.0);
}

)"