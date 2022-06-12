#version 430

uniform vec3 _CameraPosition;
uniform vec3 _lightDir;
uniform float _Roughness;
uniform float _FresnelReflectance;

in vec3 worldPosition;
in vec3 worldNormal;
in float id;
in vec3 randColor;
in vec2 uv;

#define rot(a) mat2(cos(a),-sin(a),sin(a),cos(a))

float rand(vec2 seeds){
  return fract(sin(dot(seeds, vec2(12.9898,78.233))) * 43758.5453);
}


void main(){
	vec3 col=vec3(0.0);

	float smoothness=1.0;
	float roughness=(1.0-smoothness)*(1.0-smoothness);

	vec3 viewDir=normalize(worldPosition-_CameraPosition);
	vec3 halfDir= normalize(normalize(_lightDir)+viewDir);

	float nl = clamp(dot(worldNormal, _lightDir),0.0,1.0);
    float nh = clamp(dot(worldNormal, halfDir),0.0,1.0);
    float nv = clamp(dot(worldNormal, viewDir),0.0,1.0);
    float lh = clamp(dot(_lightDir, halfDir),0.0,1.0);

	float a = roughness;
	float a2 = a*a;
	float d = nh * nh * (a2 - 1.0) + 1.00001;

	float specularTerm = a / (max(0.32, lh) * (1.5 + roughness) * d);

	col= (randColor+specularTerm*vec3(1.0))*nl+randColor*specularTerm;

	gl_FragColor=vec4(col,1.0);
}