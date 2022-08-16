R"(

#version 410

layout(points) in;
layout(triangle_strip,max_vertices =64) out;

in vec3 v2g_Normal[];
in int v2g_id[];

out vec3 WorldVertexPos;
out vec3 WorldNormal;
flat out int PrimID;
flat out int _IsStreet; // 道路かどうか
flat out int _IsSidewalk; // 歩道かどうか
flat out int g2f_IsZAxis;
out vec2 UVPerSquare; // 四角形単位のUV座標(ポリゴン二つ単位)

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
uniform float _pointNum;
uniform float _Segment;

#define rot(a) mat2(cos(a),-sin(a),sin(a),cos(a))
#define PI 3.14159265

void CreateOutputData(vec4 pos0,vec4 pos1,vec4 pos2,int IsStreet,vec3 normal,bool IsZAxis,int IsSidewalk,bool IsUpSideTriangle)
{
	gl_Position = PMatrix * VMatrix * pos0;
	WorldVertexPos = (pos0).xyz;
	WorldNormal = normalize(MMatrix*vec4(normal,0.0)).xyz;
	PrimID=v2g_id[0];
	_IsStreet=IsStreet;
	_IsSidewalk=IsSidewalk;
	g2f_IsZAxis = (IsZAxis)? 1 : 0;
	UVPerSquare = (IsUpSideTriangle)? vec2(1.0,0.0) : vec2(0.0,1.0);
	EmitVertex();

	gl_Position = PMatrix * VMatrix * pos1;
	WorldVertexPos = (pos1).xyz;
	WorldNormal = normalize(MMatrix*vec4(normal,0.0)).xyz;
	PrimID=v2g_id[0];
	_IsStreet=IsStreet;
	_IsSidewalk=IsSidewalk;
	g2f_IsZAxis = (IsZAxis)? 1 : 0;
	UVPerSquare = (IsUpSideTriangle)? vec2(1.0,1.0) : vec2(0.0,0.0);
	EmitVertex();

	gl_Position = PMatrix * VMatrix * pos2;
	WorldVertexPos = (pos2).xyz;
	WorldNormal = normalize(MMatrix*vec4(normal,0.0)).xyz;
	PrimID=v2g_id[0];
	_IsStreet=IsStreet;
	_IsSidewalk=IsSidewalk;
	g2f_IsZAxis = (IsZAxis)? 1 : 0;
	UVPerSquare = (IsUpSideTriangle)? vec2(0.0,1.0) : vec2(1.0,0.0);
	EmitVertex();

	EndPrimitive();

}

void CreateSurfaceMesh(vec4 pos,float size,int IsStreet, bool IsSide,bool IsZAxis,int IsSidewalk)
{
	vec4 pos0 =vec4(pos.xyz + vec3(-1.0 * size, 0.0, -1.0 * size),1.0);
	vec4 pos1 =vec4(pos.xyz + vec3(-1.0 * size, 0.0, 1.0 * size),1.0);
	vec4 pos2 =vec4(pos.xyz + vec3(1.0 * size, 0.0, 1.0 * size),1.0);
	vec4 pos3 =vec4(pos.xyz + vec3(1.0 * size, 0.0, -1.0 * size),1.0);
	
	CreateOutputData(pos0,pos1,pos2,IsStreet,vec3(0.0,1.0,0.0),IsZAxis,IsSidewalk,true);
	CreateOutputData(pos2,pos3,pos0,IsStreet,vec3(0.0,1.0,0.0),IsZAxis,IsSidewalk,false);
}

void CreateSideMesh(vec4 pos,float size,int IsStreet, bool IsSide,vec3 OffsetVector,float heightRate,bool IsZAxis,int IsSidewalk)
{
	vec2 IsZ=(IsZAxis)? vec2(0.0,1.0) : vec2(1.0,0.0);
	vec4 pos0 =vec4(pos.xyz + vec3(-1.0 * size*IsZ.x, -1.0 * size*heightRate,-1.0 * size*IsZ.y) + OffsetVector*size,1.0);
	vec4 pos1 =vec4(pos.xyz + vec3(-1.0 * size*IsZ.x, 1.0 * size*heightRate,-1.0 * size*IsZ.y) + OffsetVector*size,1.0);
	vec4 pos2 =vec4(pos.xyz + vec3(1.0 * size*IsZ.x, 1.0 * size*heightRate,1.0 * size*IsZ.y) + OffsetVector*size,1.0);
	vec4 pos3 =vec4(pos.xyz + vec3(1.0 * size*IsZ.x, -1.0 * size*heightRate,1.0 * size*IsZ.y) + OffsetVector*size,1.0);
	
	CreateOutputData(pos0,pos1,pos2,IsStreet,OffsetVector,IsZAxis,IsSidewalk,true);
	CreateOutputData(pos2,pos3,pos0,IsStreet,OffsetVector,IsZAxis,IsSidewalk,false);

	pos0 =vec4(pos.xyz + vec3(-1.0 * size*IsZ.x, -1.0 * size*heightRate,-1.0 * size*IsZ.y) - OffsetVector*size,1.0);
	pos1 =vec4(pos.xyz + vec3(-1.0 * size*IsZ.x, 1.0 * size*heightRate,-1.0 * size*IsZ.y) - OffsetVector*size,1.0);
	pos2 =vec4(pos.xyz + vec3(1.0 * size*IsZ.x, 1.0 * size*heightRate,1.0 * size*IsZ.y) - OffsetVector*size,1.0);
	pos3 =vec4(pos.xyz + vec3(1.0 * size*IsZ.x, -1.0 * size*heightRate,1.0 * size*IsZ.y) - OffsetVector*size,1.0);
	
	CreateOutputData(pos0,pos1,pos2,IsStreet,-OffsetVector,IsZAxis,IsSidewalk,true);
	CreateOutputData(pos2,pos3,pos0,IsStreet,-OffsetVector,IsZAxis,IsSidewalk,false);
}

void main()
{
	// 基本パラメーター
	//float pointNum = 1024.0;
	float pointNum = _pointNum;
	//float Segment = 32.0;
	float Segment = _Segment;
	float PlaneSize = 50.0;
	float SizeRate = PlaneSize/Segment;
	int IsStreet = 0;
	int IsSidewalk = 0;
	vec4 offset=vec4(0.0);
	offset.y=0.25;
	bool ZStreet=false,XStreet=false;
	//vec3 OffsetVectorZStreet = gl_in[0].gl_Position.xyz-_WorldCameraPos;
	vec3 OffsetVectorZStreet = gl_in[0].gl_Position.xyz-_ZCenterVec;
	vec3 OffsetVectorXStreet = gl_in[0].gl_Position.xyz-XSideWarkVec;

	// 距離チェック(Z軸原点ベース--> 進行方向はX方向で原点から生える)
	{
		//OffsetVectorZStreet = gl_in[0].gl_Position.xyz-_WorldCameraPos;
		if( abs(OffsetVectorZStreet.x) < StreetRadius)
		{
			offset.y += -0.1;
			IsStreet = 1;
			ZStreet=true;
		}
	}
	
	/*// 距離チェック(X軸ベース--> 進行方向はZ方向)
	{
		//OffsetVectorXStreet = gl_in[0].gl_Position.xyz-XSideWarkVec;
		if( abs(OffsetVectorXStreet.z) < StreetRadius)
		{
			if(!ZStreet)offset.y += -0.1;
			IsStreet = 1;
			XStreet=true;
		}
	}*/

	// 側面のメッシュを作成
	{
		// 側面を作成
		if(ZStreet&&!XStreet) CreateSideMesh(gl_in[0].gl_Position+offset,SizeRate,IsStreet,true, 
			normalize(vec3(OffsetVectorZStreet.x,0.0,0.0)), 0.1,true,IsSidewalk);
		if(XStreet&&!ZStreet) CreateSideMesh(gl_in[0].gl_Position+offset,SizeRate,IsStreet,true, 
			normalize(vec3(0.0,0.0,OffsetVectorXStreet.z)), 0.1,false,IsSidewalk);
	}

	// 歩道かどうかのチェック(メッシュを新たに作るのではなく、ライティングのフラグを持たせる(ひとまずZ軸ベースのみ)
	if(IsStreet!=1)
	{
		if(abs(OffsetVectorZStreet.x) >= StreetRadius && abs(OffsetVectorZStreet.x) < (StreetRadius + ToSideWarkDist) )
		{
			IsSidewalk=1;
			ZStreet=true;
		}
	}

	// 表面を作成
	CreateSurfaceMesh(gl_in[0].gl_Position+offset,SizeRate,IsStreet,false,ZStreet,IsSidewalk);
}

)"