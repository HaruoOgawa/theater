R"(

#version 410

layout(triangles) in;
layout(triangle_strip,max_vertices =12) out;

//in vec2 v2g_uv[];
in vec3 tese2g_WorldNormal[];
flat in int tese2g_id[];

//out vec2 uv;
out vec3 WorldVertexPos;
out vec3 WorldNormal;
flat out int PrimID;
flat out int _IsSidewark; // •à“¹‚©‚Ç‚¤‚©

uniform mat4 MVPMatrix;
uniform mat4 MMatrix;
uniform mat4 VMatrix;
uniform mat4 PMatrix;
uniform float _time;
uniform float _deltaTime;
uniform vec3 _WorldCameraPos;

void CreateMesh(vec4 pos0,vec4 pos1,vec4 pos2,int IsSidewark)
{
	gl_Position = PMatrix * VMatrix * pos0;
	WorldVertexPos = (pos0).xyz;
	WorldNormal = normalize(MMatrix*vec4(tese2g_WorldNormal[0],0.0)).xyz;
	//uv=v2g_uv[0];
	PrimID=tese2g_id[0];
	_IsSidewark=IsSidewark;
	EmitVertex();

	gl_Position = PMatrix * VMatrix * pos1;
	WorldVertexPos = (pos1).xyz;
	WorldNormal = normalize(MMatrix*vec4(tese2g_WorldNormal[1],0.0)).xyz;
	//uv=v2g_uv[1];
	PrimID=tese2g_id[1];
	_IsSidewark=IsSidewark;
	EmitVertex();

	gl_Position = PMatrix * VMatrix * pos2;
	WorldVertexPos = (pos2).xyz;
	WorldNormal = normalize(MMatrix*vec4(tese2g_WorldNormal[2],0.0)).xyz;
	//uv=v2g_uv[2];
	PrimID=tese2g_id[2];
	_IsSidewark=IsSidewark;
	EmitVertex();

	EndPrimitive();
}

void main()
{
	//
	vec4 pos0 = MMatrix*gl_in[0].gl_Position;
	vec4 pos1 = MMatrix*gl_in[1].gl_Position;
	vec4 pos2 = MMatrix*gl_in[2].gl_Position;

	CreateMesh(pos0,pos1,pos2,0);

	// •à“¹‚Ì\’z
	{
		float dresult = abs(pos0.x-_WorldCameraPos.x);
		dresult=min(dresult,abs(pos1.x-_WorldCameraPos.x));
		dresult=min(dresult,abs(pos2.x-_WorldCameraPos.x));

		float d = 1.5;
		vec3 offset = vec3(0.0,0.25,0.0);

		if(dresult>d)
		{
			// •à“¹‚Ì•\–Ê
			CreateMesh(
				vec4(pos0.xyz+offset,1.0),
				vec4(pos1.xyz+offset,1.0),
				vec4(pos2.xyz+offset,1.0),
				1
			);

			// •à“¹‚Ì‘¤–Ê
			if(dresult < (d+1.0))
			{
				CreateMesh(
					vec4(pos0.xyz,1.0),
					vec4(pos0.xyz+offset,1.0),
					vec4(pos1.xyz+offset,1.0),
					1
				);
				
				CreateMesh(
					vec4(pos1.xyz+offset,1.0),
					vec4(pos1.xyz,1.0),
					vec4(pos0.xyz,1.0),
					1
				);
			}
		}
	}
}

)"