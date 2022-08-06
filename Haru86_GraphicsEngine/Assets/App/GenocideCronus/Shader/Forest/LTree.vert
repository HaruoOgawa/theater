R"(

#version 410

layout(location=0)in vec3 vertex;
layout(location=1)in vec3 normal;
layout(location=2)in float treeRadius; // 半径をだんだん細くするための頂点データ。頂点がTreeのどのへんにあるか  

out vec3 LocalNormal;
out float v2g_treeRadius;

#define rot(a) mat2(cos(a),sin(a),-sin(a),cos(a))
#define PI 3.14159265

float rand(vec2 st){
    return fract(
        sin(dot(st.xy,vec2(12.9898,78.233)))*43758.5453123
    );
}

void main()
{
	//
	vec4 pos=vec4(vertex,1.0);
	float id = float(gl_InstanceID);

	// ランダムローテーション
	pos.xz*=rot(rand(vec2(id+449.212))*4.0*PI-2.0*PI);

	/*// ランダムポジション
	vec3 randPos = 200.0 * vec3(
		rand(vec2(id,0.697))*2.0-1.0,
		0.0,
		rand(vec2(1.294+id,0.412+id))*2.0-1.0
	);*/
	
	// ランダムポジション --> ひとまず半円状に広げてシーンを誤魔化す
	vec3 randPos = 200.0 * vec3(
		-1.0*rand(vec2(id,0.697)),
		0.0,
		rand(vec2(1.294+id,0.412+id))*2.0-1.0
	);

	// 中心からのオフセット
	float offradius=50.0;
	vec2 offdir= offradius * normalize(vec2(randPos.x,randPos.z));
	randPos.xz+=offdir;

	pos.xyz+=randPos;

	//
	gl_Position = pos;
	LocalNormal = normal;
	v2g_treeRadius = treeRadius;
}

)"