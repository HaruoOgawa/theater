R"(

#version 430

uniform sampler2D _BillWindowTex;
uniform float _frameResolusion;
uniform vec2 _resolution;

struct g2f{
	vec2 uv;
	vec3 normal;
};

in g2f g2f_o; 

float rand(vec2 seed){
	return fract(sin(dot(seed,vec2(12.9898,78.233)))*43758.5453);
}

void main(){
	vec3 col=vec3(1.0);
	
	vec2 st=g2f_o.uv;
	st.y*=(_resolution.x/_resolution.y);
	st=st*2.0-1.0;
	vec2 domainID=floor(st*10.0);
	st=fract(st*10.0);
	
	// Šiqì¬
	col.rgb*=( length(max(vec2(0.0),abs(st)-vec2(0.9))) < 0.001 )? 1.0 : 0.0;
	
	// ‘‹‚Ì–¾‚©‚è
	col*=vec3(1.0,1.0,1.0)*rand(vec2(domainID.x*10.0,domainID.y*10.0));
	col*=( rand(vec2(domainID.x*10.0,domainID.y*10.0))>0.9 )? 1.5*vec3(1.0,1.0,2.0) : vec3(1.0);

	// y•ûŒü‚É‚Í‘‹‚ğ•`‚©‚È‚¢
	vec3 n=normalize(g2f_o.normal);
	float ndy=abs(dot(n,vec3(0.0,1.0,0.0)));
	col*=(1.0-ndy);
	
	gl_FragColor=vec4(col,1.0);
}

)"