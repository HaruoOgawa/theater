R"(

#version 410

layout(triangles) in;
layout(triangle_strip,max_vertices=10) out;

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

in float t2g_id[];
in vec3 t2g_normal[];
in vec4 t2g_billinfo[];
in vec3 t2g_WorldVertexPos[];

flat out float g2f_id;
out vec3 g2f_normal;
flat out vec4 g2f_billinfo;
out vec3 g2f_WorldVertexPos;

// hash --> https://www.shadertoy.com/view/4dffRH
vec3 hash( vec3 p ) // replace this by something better. really. do
{
	p = vec3( dot(p,vec3(127.1,311.7, 74.7)),
			  dot(p,vec3(269.5,183.3,246.1)),
			  dot(p,vec3(113.5,271.9,124.6)));

	return -1.0 + 2.0*fract(sin(p)*43758.5453123);
}

void Createvertex(vec3 offset)
{
	mat4 nMat = transpose(inverse(MMatrix));

	gl_Position = MVPMatrix * (gl_in[0].gl_Position + vec4(offset,0.0));
	g2f_id=t2g_id[0];
	g2f_WorldVertexPos = (MMatrix* gl_in[0].gl_Position).xyz;
	g2f_normal = normalize( nMat*vec4(t2g_normal[0],0.0) ).xyz;
	g2f_billinfo = t2g_billinfo[0];

	EmitVertex();

	gl_Position = MVPMatrix * (gl_in[1].gl_Position + vec4(offset,0.0));
	g2f_id=t2g_id[1];
	g2f_WorldVertexPos = (MMatrix* gl_in[1].gl_Position).xyz;
	g2f_normal = normalize( nMat*vec4(t2g_normal[1],0.0) ).xyz;
	g2f_billinfo = t2g_billinfo[1];

	EmitVertex();

	gl_Position = MVPMatrix * (gl_in[2].gl_Position + vec4(offset,0.0));
	g2f_id=t2g_id[2];
	g2f_WorldVertexPos = (MMatrix* gl_in[2].gl_Position).xyz;
	g2f_normal = normalize( nMat*vec4(t2g_normal[2],0.0) ).xyz;
	g2f_billinfo = t2g_billinfo[2];

	EmitVertex();

	EndPrimitive();
}

void main()
{
	// ランダムポジション
	float id = t2g_id[0];
	float domainSize=32.0;
	vec2 domainID=vec2(0.0);
	domainID.y=floor(id/domainSize);
	domainID.x=id-domainID.y*domainSize;
	
	vec3 randPos=vec3(0.0);
	float randPosRadius=75.0;
	randPos=hash(vec3(id+7.22,id+id,id-88.21))*randPosRadius;
	randPos.y=1.5;

	randPos.z-=_time*10.0;
	randPos.z=mod(randPos.z,randPosRadius)-randPosRadius*0.5;

	// 大通りのぶんだけ道を開ける
	{
		vec3 OffsetVectorZStreet = randPos-_ZCenterVec;

		if(abs(OffsetVectorZStreet.x) <= (StreetRadius + ToSideWarkDist))
		{
			vec3 StreetOffVec = exp(-0.3*abs(OffsetVectorZStreet.x))*ToSideWarkDist * StreetRadius * normalize(vec3( OffsetVectorZStreet.x ,0.0,0.0));
			randPos+=StreetOffVec;
		}
	}

	/*// X軸の大通りの分だけ、道をあける
	{
		vec3 OffsetVectorXStreet = randPos-XSideWarkVec;
		if( abs(OffsetVectorXStreet.z) < StreetRadius)
		{
			vec3 StreetOffVec = StreetRadius * normalize( vec3(0.0,0.0,1.5*(OffsetVectorXStreet.z)) );
			randPos+=StreetOffVec;
		}
	}*/

	Createvertex(randPos);
}

)"