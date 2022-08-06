R"(
#version 330

in vec2 uv;
uniform vec2 _resolution;

float rand(vec2 seed){
 return fract(sin(dot(seed,vec2(12.9898,78.233)))*43758.5453);
}

void main(){
	vec3 col=vec3(1.0);
	
	vec2 st=uv;
	st.x*=(_resolution.x/_resolution.y);
	st=st*2.0-1.0;
	vec2 domainID=floor(st*10.0);
	st=fract(st*10.0);
	
	// äiéqçÏê¨
	col.rgb*=( length(max(vec2(0.0),abs(st)-vec2(0.9))) < 0.001 )? 1.0 : 0.0;
	
	// ëãÇÃñæÇ©ÇË
	col*=vec3(1.0,1.0,1.0)*rand(vec2(domainID.x,domainID.y))*0.5;

	gl_FragColor=vec4(col,1.0);
}

)"