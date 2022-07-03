R"(

#version 330

uniform sampler2D _WaterRP;

in vec2 uv;


void main(){
	vec3 col=vec3(0.0);
	col=texture(_WaterRP,uv).rgb;
	//col=vec3(uv,0.0);
	gl_FragColor=vec4(col,1.0);
}

)"