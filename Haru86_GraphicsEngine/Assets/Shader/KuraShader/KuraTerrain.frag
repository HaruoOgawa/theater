#version 410

in float pid;
in vec3 g2f_normal;
in vec2 g2f_uv;

uniform sampler2D _terrainMap;

#define LightDir vec3(-0.613121, -0.1916, 0.766402)

float rand(vec2 seeds){
  return fract(sin(dot(seeds, vec2(12.9898,78.233))) * 43758.5453);
}

void main(){
	vec4 col=vec4(1.0);
	col.rgb=vec3(g2f_uv,0.0);
	col.rgb=texture(_terrainMap,g2f_uv).rgb;

	float diff=max(dot(normalize(g2f_normal),normalize(LightDir)),0.25);
	col.rgb*=diff;
	
	gl_FragColor=col;
}