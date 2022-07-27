R"(

#version 430

layout(location=0)in vec3 vertex;
layout(location=1)in vec3 normal;
layout(location=2)in float treeRadius; // 半径をだんだん細くするための頂点データ。頂点がTreeのどのへんにあるか  

out vec3 LocalNormal;
out float v2g_treeRadius;

float rand(vec2 st){
    return fract(
        sin(dot(st.xy,vec2(12.9898,78.233)))*43758.5453123
    );
}

void main()
{
	float id = float(gl_InstanceID);

	vec3 randPos = 100.0 * vec3(
		rand(vec2(id,0.697))*2.0-1.0,
		0.0,
		rand(vec2(1.294+id,0.412+id))*2.0-1.0
	);

	float offradius=100.0;
	vec2 offdir= offradius * normalize(vec2(randPos.x,randPos.z));
	randPos.xz+=offdir;

	gl_Position = vec4(vertex+randPos,1.0);
	LocalNormal = normal;
	v2g_treeRadius = treeRadius;
}

)"