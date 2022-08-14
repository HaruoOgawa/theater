R"(

#version 410

uniform float _time;
uniform float _deltaTime;
uniform vec3 _CameraPos;

layout(location=0)in vec3 vertex;
layout(location=1)in vec3 normal;
layout(location=2)in vec2 texcoord;
layout(location=3)in vec4 weights;
layout(location=4)in ivec4 joints;

struct v2g{
	vec4 position;
	vec2 uv;
	mat4 RandPosMat;
	mat4 LocalMatrix;
	int id;
};

out v2g v2g_o; 

#define rot(a) mat2(cos(a),-sin(a),sin(a),cos(a))
#define PI 3.14159265

float rand(vec2 seeds){
  return fract(sin(dot(seeds, vec2(12.9898,78.233))) * 43758.5453);
}

// hash --> https://www.shadertoy.com/view/4dffRH
vec3 hash( vec3 p ) // replace this by something better. really. do
{
	p = vec3( dot(p,vec3(127.1,311.7, 74.7)),
			  dot(p,vec3(269.5,183.3,246.1)),
			  dot(p,vec3(113.5,271.9,124.6)));

	return -1.0 + 2.0*fract(sin(p)*43758.5453123);
}

mat4 CalRotMatrix(vec3 a)
{
	mat4 result;

	mat4 rotx=mat4(
		vec4(1.0,0.0,0.0,0.0),
		vec4(0.0,cos(a.x),-sin(a.x),0.0),
		vec4(0.0,sin(a.x),cos(a.x),0.0),
		vec4(0.0,0.0,0.0,1.0)
	);

	mat4 roty=mat4(
		vec4(cos(a.y),0.0,sin(a.y),0.0),
		vec4(0.0,1.0,0.0,0.0),
		vec4(-sin(a.y),0.0,cos(a.y),0.0),
		vec4(0.0,0.0,0.0,1.0)
	);
	
	mat4 rotz=mat4(
		vec4(cos(a.z),-sin(a.z),0.0,0.0),
		vec4(sin(a.z),cos(a.z),0.0,0.0),
		vec4(0.0,0.0,1.0,0.0),
		vec4(0.0,0.0,0.0,1.0)
	);

	result = rotz * roty * rotx;

	return result;
}

void main(){
	// 基本パラメーター
	int id=gl_InstanceID;
	float id_f=float(id);
	vec2 domainID=vec2(0.0);
	domainID.y=floor(id_f/32.0);
	domainID.x=id_f-domainID.y*32.0;
	vec4 pos=vec4(vertex,1.0);
	
	// ランダムワールドポジション
	float height = 50.0;
	vec3 randPos=vec3(0.0);
	randPos=( vec3( 
		rand( vec2(float(gl_InstanceID),0.321)), 
		rand( vec2(float(gl_InstanceID),0.11159)),
		rand( vec2(float(gl_InstanceID),0.741))
	) * 2.0-1.0 )*height*0.5;
	//randPos.y+=25.0+sin(_time*5.0+rand(domainID)*3.14*2.0)*2.0;
	//randPos.y=mod(randPos.y+_time*10.0,height)-height*0.5;
	
	// ランダムスケール
	//vec3 randScale = (hash(vec3(id_f+7.77123,id_f+id_f+id_f,id_f*6.2+1.01))*0.5+0.5)*5.0;
	vec3 randScale = vec3(rand(vec2(id_f+7.77123,id_f+id_f+id_f))*2.0);
	mat4 RandScaleMat = mat4(
		vec4(randScale.x,0.0,0.0,0.0),
		vec4(0.0,randScale.y,0.0,0.0),
		vec4(0.0,0.0,randScale.z,0.0),
		vec4(0.0,0.0,0.0,1.0)
	);

	// ランダムローテーション(回転行列の定義がめんどくさいのでここで先に計算する)
	vec3 randAngle = hash(vec3(id_f+6.14,id_f+1.111,id_f+45.69))*PI*2.0;

	mat4 RandRotateMat = CalRotMatrix(randAngle+randAngle*vec3(_time*0.5));

	// ransPosのオフセット方向を決定する(基本上に直進させたい --> やっぱランダムで)
	vec4 OffDir =vec4( vec3(0.0,1.0,0.0) ,0.0);
	//vec4 OffDir =vec4( normalize(hash(vec3(id_f*0.01+id_f,0.621,id_f+9.99+id_f*4.0))) ,0.0);
	//OffDir=RandRotateMat*OffDir;
	
	randPos+=normalize(OffDir.xyz)*_time;
	randPos=mod(randPos,height)-height*0.5;
	mat4 RandPosMat = mat4(
		vec4(0.0,0.0,0.0,randPos.x),
		vec4(0.0,0.0,0.0,randPos.y),
		vec4(0.0,0.0,0.0,randPos.z),
		vec4(0.0,0.0,0.0,1.0)
	);
	//RandPosMat = transpose(RandPosMat);

	// ローカルマトリックスを作成(次のシェーダーの変換に使用するためのもの)
	mat4 LocalMatrix = RandRotateMat * RandScaleMat;

	// アウトプット
	gl_Position=pos;
	v2g_o.position=pos;
	v2g_o.uv=texcoord;
	v2g_o.RandPosMat=transpose(RandPosMat);
	v2g_o.LocalMatrix=transpose(LocalMatrix);
	v2g_o.id=id;
}

)"