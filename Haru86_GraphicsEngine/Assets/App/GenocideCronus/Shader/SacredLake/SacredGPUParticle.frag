R"(

#version 410

uniform sampler2D _BillWindowTex;
uniform float _frameResolusion;
uniform vec2 _resolution;

struct g2f{
	vec2 uv;
	float id;
	float billID;
};

in g2f g2f_o; 
in vec3 WorldNormal;

float rand(vec2 seeds){
  return fract(sin(dot(seeds, vec2(12.9898,78.233))) * 43758.5453);
}

void main(){
	vec3 col=vec3(1.0);
	vec3 lightDir=normalize(vec3(1.0,-1.0,1.0));
	float diff=max( 0.0,dot(lightDir,WorldNormal) );
	col*=diff;
	
	//col=WorldNormal*0.5+0.5;
	//col=vec3(g2f_o.uv,0.0);

	gl_FragColor=vec4(col,1.0);
}

)"