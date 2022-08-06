R"(

#version 410

uniform sampler2D _BillWindowTex;
uniform float _frameResolusion;
uniform vec2 _resolution;

struct g2f{
	vec2 uv;
	vec3 normal;
	float id;
	float billID;
};

in g2f g2f_o; 

float rand(vec2 seeds){
  return fract(sin(dot(seeds, vec2(12.9898,78.233))) * 43758.5453);
}

vec2 random2(vec2 seeds)
{
 seeds = vec2(dot(seeds, vec2(127.1, 311.7)),
 dot(seeds, vec2(269.5, 183.3)));
 return fract(sin(seeds) * 43758.5453123);
}

float perlinNoise(vec2 seeds)
{
vec2 i = floor(seeds);
vec2 f = fract(seeds);
vec2 i00 = i + vec2(0, 0);
vec2 i10 = i + vec2(1, 0);
vec2 i01 = i + vec2(0, 1);
vec2 i11 = i + vec2(1, 1);
vec2 f00 = f - vec2(0, 0);
vec2 f10 = f - vec2(1, 0);
vec2 f01 = f - vec2(0, 1);
vec2 f11 = f - vec2(1, 1);
vec2 g00 = normalize(-1.0 + 2.0 * random2(i00));
vec2 g10 = normalize(-1.0 + 2.0 * random2(i10));
vec2 g01 = normalize(-1.0 + 2.0* random2(i01));
vec2 g11 = normalize(-1.0 + 2.0* random2(i11));
float v00 = dot(g00, f00);
float v10 = dot(g10, f10);
float v01 = dot(g01, f01);
float v11 = dot(g11, f11);
vec2 p = smoothstep(0.0, 1.0, f);
float v00v10 = mix(v00, v10, p.x);
float v01v11 = mix(v01, v11, p.x);
return mix(v00v10, v01v11, p.y) * 0.5 + 0.5;
}

void main(){
	vec3 col=vec3(1.0);
	
	float rep=30.0;
	vec2 st=g2f_o.uv;
	st.x*=(_resolution.y/_resolution.x);
	st=st*2.0-1.0;
	vec2 domainID=floor(st*rep);

	//rep=(g2f_o.billID!=2.0)? rep : rep*0.5;
	st=fract(st*rep);
	
	// äiéqçÏê¨
	col.rgb*=( length(max(vec2(0.0),abs(st)-vec2(0.9))) < 0.001 )? 1.0 : 0.0;
	//col+=vec3(perlinNoise(g2f_o.uv*100.0));

	// ëãÇÃñæÇ©ÇË
	col*=vec3(1.0,1.0,1.0)*rand(vec2(domainID.x*rep,domainID.y*rep));

	float isTurnOn=rand(vec2(domainID.y,g2f_o.id));
	isTurnOn*=( (domainID.x < rand(vec2(g2f_o.id,isTurnOn))*rep)? 1.0 : 0.0 );

	//col*=( isTurnOn>0.85 )? 1.5*vec3(1.0,1.0,2.0) : vec3(1.0)*0.15;
	col*=( isTurnOn>0.85 )? 3.0*vec3(1.0,1.0,0.95) : vec3(1.0)*0.15;

	// yï˚å¸Ç…ÇÕëãÇï`Ç©Ç»Ç¢
	vec3 n=normalize(g2f_o.normal);
	float ndy=abs(dot(n,vec3(0.0,1.0,0.0)));
	col*=(1.0-ndy);
	
	gl_FragColor=vec4(col,0.8);
}

)"