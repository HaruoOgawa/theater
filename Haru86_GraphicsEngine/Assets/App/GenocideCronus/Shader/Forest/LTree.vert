R"(

#version 430

layout(location=0)in vec3 vertex;
layout(location=1)in vec3 normal;
layout(location=2)in float treeRadius; // 半径をだんだん細くするための頂点データ。頂点がTreeのどのへんにあるか  

out vec3 LocalNormal;
out float v2g_treeRadius;

void main()
{
	gl_Position = vec4(vertex,1.0);
	LocalNormal = normal;
	v2g_treeRadius = treeRadius;
}

)"