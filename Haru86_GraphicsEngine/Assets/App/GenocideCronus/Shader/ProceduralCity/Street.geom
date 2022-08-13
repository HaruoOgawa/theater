R"(

#version 410

layout(points) in;
layout(triangle_strip,max_vertices =64) out;

in vec3 v2g_Normal[];
in int v2g_id[];

out vec3 WorldVertexPos;
out vec3 WorldNormal;
flat out int PrimID;
flat out int _IsSidewark; // 歩道かどうか

uniform mat4 MVPMatrix;
uniform mat4 MMatrix;
uniform mat4 VMatrix;
uniform mat4 PMatrix;
uniform float _time;
uniform float _deltaTime;
uniform vec3 _WorldCameraPos;

uniform vec3 XSideWarkVec;
uniform float StreetRadius;

#define rot(a) mat2(cos(a),-sin(a),sin(a),cos(a))
#define PI 3.14159265

void CreateOutputData(vec4 pos0,vec4 pos1,vec4 pos2,int IsSidewark,vec3 normal)
{
	gl_Position = PMatrix * VMatrix * pos0;
	WorldVertexPos = (pos0).xyz;
	WorldNormal = normalize(MMatrix*vec4(normal,0.0)).xyz;
	PrimID=v2g_id[0];
	_IsSidewark=IsSidewark;
	EmitVertex();

	gl_Position = PMatrix * VMatrix * pos1;
	WorldVertexPos = (pos1).xyz;
	WorldNormal = normalize(MMatrix*vec4(normal,0.0)).xyz;
	PrimID=v2g_id[0];
	_IsSidewark=IsSidewark;
	EmitVertex();

	gl_Position = PMatrix * VMatrix * pos2;
	WorldVertexPos = (pos2).xyz;
	WorldNormal = normalize(MMatrix*vec4(normal,0.0)).xyz;
	PrimID=v2g_id[0];
	_IsSidewark=IsSidewark;
	EmitVertex();

	EndPrimitive();

}

void CreateSurfaceMesh(vec4 pos,float size,int IsSidewark, bool IsSide)
{
	vec4 pos0 =vec4(pos.xyz + vec3(-1.0 * size, 0.0, -1.0 * size),1.0);
	vec4 pos1 =vec4(pos.xyz + vec3(-1.0 * size, 0.0, 1.0 * size),1.0);
	vec4 pos2 =vec4(pos.xyz + vec3(1.0 * size, 0.0, 1.0 * size),1.0);
	vec4 pos3 =vec4(pos.xyz + vec3(1.0 * size, 0.0, -1.0 * size),1.0);
	
	CreateOutputData(pos0,pos1,pos2,IsSidewark,vec3(0.0,1.0,0.0));
	CreateOutputData(pos2,pos3,pos0,IsSidewark,vec3(0.0,1.0,0.0));
}

void CreateSideMesh(vec4 pos,float size,int IsSidewark, bool IsSide,vec3 OffsetVector,float heightRate,bool IsZAxis)
{
	vec2 IsZ=(IsZAxis)? vec2(0.0,1.0) : vec2(1.0,0.0);
	vec4 pos0 =vec4(pos.xyz + vec3(-1.0 * size*IsZ.x, -1.0 * size*heightRate,-1.0 * size*IsZ.y) + OffsetVector*size,1.0);
	vec4 pos1 =vec4(pos.xyz + vec3(-1.0 * size*IsZ.x, 1.0 * size*heightRate,-1.0 * size*IsZ.y) + OffsetVector*size,1.0);
	vec4 pos2 =vec4(pos.xyz + vec3(1.0 * size*IsZ.x, 1.0 * size*heightRate,1.0 * size*IsZ.y) + OffsetVector*size,1.0);
	vec4 pos3 =vec4(pos.xyz + vec3(1.0 * size*IsZ.x, -1.0 * size*heightRate,1.0 * size*IsZ.y) + OffsetVector*size,1.0);
	
	CreateOutputData(pos0,pos1,pos2,IsSidewark,OffsetVector);
	CreateOutputData(pos2,pos3,pos0,IsSidewark,OffsetVector);

	pos0 =vec4(pos.xyz + vec3(-1.0 * size*IsZ.x, -1.0 * size*heightRate,-1.0 * size*IsZ.y) - OffsetVector*size,1.0);
	pos1 =vec4(pos.xyz + vec3(-1.0 * size*IsZ.x, 1.0 * size*heightRate,-1.0 * size*IsZ.y) - OffsetVector*size,1.0);
	pos2 =vec4(pos.xyz + vec3(1.0 * size*IsZ.x, 1.0 * size*heightRate,1.0 * size*IsZ.y) - OffsetVector*size,1.0);
	pos3 =vec4(pos.xyz + vec3(1.0 * size*IsZ.x, -1.0 * size*heightRate,1.0 * size*IsZ.y) - OffsetVector*size,1.0);
	
	CreateOutputData(pos0,pos1,pos2,IsSidewark,-OffsetVector);
	CreateOutputData(pos2,pos3,pos0,IsSidewark,-OffsetVector);
}

void main()
{
	// 基本パラメーター
	float pointNum = 1024.0;
	float Segment = 32.0;
	float PlaneSize = 50.0;
	float SizeRate = PlaneSize/Segment;
	int IsSidewark = 0;
	vec4 offset=vec4(0.0);
	offset.y=0.25;
	//float ToSideWarkDist = 2.5;
	bool ZStreet=false,XStreet=false;
	vec3 OffsetVectorZStreet,OffsetVectorXStreet;

	// 距離チェック(Z軸原点ベース--> 進行方向はX方向で原点から生える)
	{
		OffsetVectorZStreet = gl_in[0].gl_Position.xyz-_WorldCameraPos;
		if( abs(OffsetVectorZStreet.x) < StreetRadius)
		{
			offset.y += -0.1;
			IsSidewark = 1;
			ZStreet=true;
		}
	}
	
	// 距離チェック(X軸ベース--> 進行方向はZ方向)
	{
		//vec3 XSideWarkVec = vec3(0.0,0.0,mod(-_time*10.0,PlaneSize)-PlaneSize*0.5);
		OffsetVectorXStreet = gl_in[0].gl_Position.xyz-XSideWarkVec;
		if( abs(OffsetVectorXStreet.z) < StreetRadius)
		{
			if(!ZStreet)offset.y += -0.1;
			IsSidewark = 1;
			XStreet=true;
		}
	}

	// 側面のメッシュを作成
	{
		// 側面を作成
		if(ZStreet&&!XStreet) CreateSideMesh(gl_in[0].gl_Position+offset,SizeRate,IsSidewark,true, normalize(vec3(OffsetVectorZStreet.x,0.0,0.0)), 0.1,true);
		if(XStreet&&!ZStreet) CreateSideMesh(gl_in[0].gl_Position+offset,SizeRate,IsSidewark,true, normalize(vec3(0.0,0.0,OffsetVectorXStreet.z)), 0.1,false);
	}

	// 表面を作成
	CreateSurfaceMesh(gl_in[0].gl_Position+offset,SizeRate,IsSidewark,false);
}

)"