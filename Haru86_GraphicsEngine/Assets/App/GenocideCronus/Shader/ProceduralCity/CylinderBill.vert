R"(

#version 410

uniform mat4 MVPMatrix;
uniform mat4 MMatrix;
uniform mat4 VMatrix;
uniform mat4 PMatrix;
uniform float _time;
uniform float _deltaTime;
uniform vec3 _WorldCameraPos;

uniform vec3 XSideWarkVec;
uniform float StreetRadius;
uniform float ToSideWarkDist;
uniform vec3 _ZCenterVec; // Zは無限大である

layout(location=0)in vec3 vertex;
layout(location=1)in vec3 normal;
layout(location=2)in vec4 billinfo;

out vec3 v2f_normal;
out vec3 v2f_WorldVertexPos;
flat out vec4 v2f_billinfo;
flat out float v2f_id;

#define rot(a) mat2(cos(a),-sin(a),sin(a),cos(a))
#define PI 3.14159265

const float VerDelimiterNum = 10.0;
const float BillRadius = 5.0;
const float AdjustVal = 0.1;

// hash --> https://www.shadertoy.com/view/4dffRH
vec3 hash( vec3 p ) // replace this by something better. really. do
{
	p = vec3( dot(p,vec3(127.1,311.7, 74.7)),
			  dot(p,vec3(269.5,183.3,246.1)),
			  dot(p,vec3(113.5,271.9,124.6)));

	return -1.0 + 2.0*fract(sin(p)*43758.5453123);
}

 float rand(vec2 st)
{
    return fract(sin(dot(st, vec2(12.9898, 78.233))) * 43758.5453);
}

void main(){
	// 基本パラメーター
	//float id = floor(_time*0.15);
	float id = float(gl_InstanceID+1024+1);
	vec4 pos=vec4(vertex,1.0);
	vec4 localNormal = vec4(normal,0.0);
	
	// ビルデータを整理
	bool IsWindow = (billinfo.x == 1.0);
	bool IsVertical = (billinfo.y == 1.0);
	float DilimiterOrder = billinfo.z;
	bool IsXAxis = (billinfo.w == 1.0);

	// 区切りの幅を乱数で調整
	if(!IsWindow && IsVertical)
	{
		// 乱数を準備
		vec3 noise0 = hash(vec3(id+3.3321,id+941.2,18.52));
		float StretchWidth = noise0.z*0.5+0.5+0.25;
		
		//
		float myAngle = acos( dot(vec3(1.0,0.0,0.0),normalize(vec3(vertex.x,0.0,vertex.z))) );
		float startAngle = min(noise0.x*PI,noise0.y*PI);
		float endAngle = max(noise0.x*PI,noise0.y*PI);

		// 軸が揃っている方向のみ引き延ばす
		if(myAngle>=startAngle && myAngle<endAngle)
		{
			//
			mat4 StretchMatrix = mat4(
				vec4(1.0,0.0,0.0,0.0),
				vec4(0.0,1.0,0.0,0.0),
				vec4(0.0,0.0,1.0,0.0),
				vec4(0.0,0.0,0.0,1.0)
			);

			StretchMatrix[0][0] = StretchWidth;
			StretchMatrix[2][2] = StretchWidth;

			// 行列をかける
			pos = StretchMatrix * pos;
			localNormal = StretchMatrix * localNormal;
		}
	}

	// ランダムな回転。ただし回転値は PI/2.0の倍数のみ
	{
		float randRotation = floor( (rand(vec2(id+0.556,id+id))*2.0-1.0) *10.0) * (PI/2.0);
		pos.xz*=rot(randRotation);
		localNormal.xz*=rot(randRotation);
	}

	// 乱数で全体の高さ・幅を決める
	{
		vec3 noise2 = hash(vec3(0.11195,id+id+9.4,id-63.14));
		noise2=noise2*0.5+0.5;
		noise2+=1.0;

		mat4 ScaleMatrix = mat4(
			vec4(noise2.x,0.0,0.0,0.0),
			vec4(0.0,noise2.y*2.0,0.0,0.0),
			vec4(0.0,0.0,noise2.x,0.0),
			vec4(0.0,0.0,0.0,1.0)
		);

		pos = ScaleMatrix * pos;
	}

	// ランダムポジション
	{
		// ランダムポジション
		vec3 randPos=vec3(0.0);
		float randPosRadius=75.0;
		randPos=hash(vec3(id+7.22,id+id,id-88.21))*randPosRadius;
		randPos.y=0.0;

		randPos.z-=_time*10.0;
		randPos.z=mod(randPos.z,randPosRadius)-randPosRadius*0.5;

		// 大通りのぶんだけ道を開ける
		{
			vec3 StreetOffVec = ToSideWarkDist * StreetRadius * normalize(vec3( (randPos.x-_ZCenterVec.x) ,0.0,0.0));
			randPos+=StreetOffVec;
		}

		//
		pos.xyz+=randPos;
	}

	// アウトプット
	gl_Position=MVPMatrix*pos;
	v2f_WorldVertexPos=(MMatrix*pos).xyz;
	v2f_normal=normalize( (MMatrix*vec4(normalize(localNormal.xyz),0.0)).xyz );
	v2f_billinfo=billinfo;
	v2f_id=id;
}

)"