R"(

#version 410

uniform float _time;
uniform float _deltaTime;
uniform vec3 _CameraPos;
uniform mat4 MVPMatrix;
uniform mat4 MMatrix;
uniform mat4 VMatrix;
uniform mat4 PMatrix;

uniform int _IDOffset;
uniform float _ParticleScale;
uniform int _IsRandomScale;
uniform float _ParticleMoveSpeed;

layout(location=0)in vec3 vertex;
layout(location=1)in vec3 normal;
layout(location=2)in vec2 texcoord;
layout(location=3)in vec4 weights;
layout(location=4)in ivec4 joints;

out vec2 uv;
out vec3 WorldVertexPos;
out vec3 WorldNormal;

/*struct v2g{
	vec4 position;
	vec2 uv;
	mat4 RandPosMat;
	mat4 LocalMatrix;
	int id;
};

out v2g v2g_o; */

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
	float id=float(gl_InstanceID+_IDOffset);
	vec2 domainID=vec2(0.0);
	domainID.y=floor(id/32.0);
	domainID.x=id-domainID.y*32.0;
	vec4 pos=vec4(vertex,1.0);
	
	// ランダムスケール
	vec3 randScale = vec3(1.0); 
	if(_IsRandomScale != 1)
	{
		randScale = vec3(rand(vec2(id+7.77123,id+id+id))*_ParticleScale);
	}
	else
	{
		randScale = hash( vec3(id+0.6121+float(gl_VertexID), id+id*10.0+float(gl_VertexID), id+41.253+float(gl_VertexID)) )*_ParticleScale;
	}

	mat4 RandScaleMat = mat4(
		vec4(randScale.x,0.0,0.0,0.0),
		vec4(0.0,randScale.y,0.0,0.0),
		vec4(0.0,0.0,randScale.z,0.0),
		vec4(0.0,0.0,0.0,1.0)
	);

	// ランダムローテーション(回転行列の定義がめんどくさいのでここで先に計算する)
	vec3 randAngle = hash(vec3(id+6.14,id+1.111,id+45.69))*PI*2.0;
	mat4 RandRotateMat = CalRotMatrix(randAngle+randAngle*vec3(_time*0.5));

	// ransPosのオフセット方向を決定する(基本上に直進させたい --> やっぱランダムで)
	vec4 OffDir =vec4( vec3(0.0, 0.1+rand(vec2(id*0.01+id,0.621)) ,0.0) ,0.0);
	//vec4 OffDir =vec4( normalize(hash(vec3(id*0.01+id,0.621,id+9.99+id*4.0))) ,0.0);
	//OffDir=RandRotateMat*OffDir;
	
	// ランダムワールドポジション
	float height = 100.0;
	float width = 60.0;
	vec3 randPos=vec3(0.0);
	randPos=( vec3( 
		rand( vec2(float(gl_InstanceID),0.321)), 
		rand( vec2(float(gl_InstanceID),0.11159)),
		rand( vec2(float(gl_InstanceID),0.741))
	) * 2.0-1.0 );
	randPos.y*=height;
	randPos.xz*=width;

	randPos+=normalize(OffDir.xyz)*_time*_ParticleMoveSpeed;
	randPos.y=mod(randPos.y,height);
	mat4 RandPosMat = mat4(
		vec4(0.0,0.0,0.0,randPos.x),
		vec4(0.0,0.0,0.0,randPos.y),
		vec4(0.0,0.0,0.0,randPos.z),
		vec4(0.0,0.0,0.0,1.0)
	);

	// ローカルマトリックスを作成(次のシェーダーの変換に使用するためのもの)
	mat4 LocalMatrix = /*RandPosMat **/ RandRotateMat * RandScaleMat;
	
	pos=LocalMatrix*pos;
	pos.xyz+=randPos;

	// アウトプット
	gl_Position=MVPMatrix*pos;
	WorldNormal=((RandRotateMat)*vec4(normal,0.0)).xyz;
	WorldVertexPos=(MMatrix*pos).xyz;
	uv=texcoord;
}

)"