R"(

#version 410

in vec3 WorldVertexPos;
in vec3 WorldNormal;
flat in int PrimID;
flat in int _IsStreet; // 道路かどうか
flat in int _IsSidewalk; // 歩道かどうか
flat in int g2f_IsZAxis;

uniform int _UseColor;
uniform vec4 _Color;
uniform int _UseLighting;
uniform vec3 _LightDir;
uniform vec3 _LightPos;
uniform int _UseEnvColor;
uniform vec4 _EnvColor;
uniform vec3 _WorldCameraPos;
uniform float _time;

uniform vec3 XSideWarkVec;
uniform float StreetRadius;

uniform int _UseMainTex;
uniform sampler2D _MainTex;
//uniform samplerCube _MainTex;

vec3 hash( vec3 p ) // replace this by something better. really. do
{
	p = vec3( dot(p,vec3(127.1,311.7, 74.7)),
			  dot(p,vec3(269.5,183.3,246.1)),
			  dot(p,vec3(113.5,271.9,124.6)));

	return -1.0 + 2.0*fract(sin(p)*43758.5453123);
}

void main(){
	vec4 col=vec4(vec3(0.0),1.0);

	// ベースカラー
	if(_UseColor == 1)
	{
		col=_Color;
	}
	else if(_UseMainTex == 1) // テクスチャサンプリング
	{
		//col=texture(_MainTex,uv);
		/*vec3 viewdir = normalize(_WorldCameraPos-WorldVertexPos);
		vec3 rpdir = normalize(reflect(viewdir,WorldNormal));
		col=texture(_MainTex,rpdir);*/

		//col=vec4(rpdir*0.5+0.5,1.0);
	}
	else
	{
		col=vec4(1.0);
		//col=vec4(uv.x,uv.y,0.0,1.0);
	}

	

	// 環境光
	vec4 envColor = vec4(0.0,0.0,0.0,1.0);
	if(_UseEnvColor == 1)
	{
		envColor = _EnvColor;
	}

	// ライティング
	if(_UseLighting == 1)
	{
		//vec3 lightDir=normalize(_LightDir);
		vec3 lightDir=normalize(_LightPos-WorldVertexPos);
		float diff=max(0.0,dot(WorldNormal,lightDir));
		col.rgb*=diff;

		col.rgb+=envColor.rgb;

		vec3 viewDir= -1.0*normalize(WorldVertexPos-_WorldCameraPos);
		vec3 halfDir=normalize(viewDir + lightDir);
		float spec=pow( max(0.0,dot(WorldNormal,halfDir)) , 60.0);
		col.rgb+=vec3(1.0)*spec;

		//col.rgb=viewDir*0.5+0.5;
	}

	//col.rgb=WorldNormal*0.5+0.5;
	//if(_IsStreet==1) col.rgb=hash(vec3( float(PrimID)+0.0012,float(PrimID)+float(PrimID)+6.7777,float(PrimID)+1.2396 ))*0.5+0.5;

	// 道路のライティング
	if(_IsStreet==1)
	{
		float StreetLineWidth = 0.05;
		vec2 StreetUV = vec2(0.0);
	
		if(g2f_IsZAxis == 1)
		{
			StreetUV.x = clamp(abs(WorldVertexPos.x-_WorldCameraPos.x)/1.5,0.0,1.0);

			col.rgb += vec3( (StreetUV.x<StreetLineWidth)? 1.0 : 0.0);
			col.rgb += vec3( (StreetUV.x>1.0-StreetLineWidth && StreetUV.x<1.0)? 1.0 : 0.0);
			col.rgb += vec3( (StreetUV.x>0.5-StreetLineWidth*0.5 && StreetUV.x<0.5+StreetLineWidth*0.5)? 1.0 : 0.0)
				* ( (int(floor(WorldVertexPos.z * 2.0 + _time*10.0))%2==0)? 1.0 : 0.0 ); // 点線にする(止まって見えるのでスクロールする)

		}
		else if(g2f_IsZAxis == 0)
		{
			StreetUV.y = clamp(abs(WorldVertexPos.z-XSideWarkVec.z)/1.5,0.0,1.0);

			col.rgb += vec3( (StreetUV.y<StreetLineWidth)? 1.0 : 0.0);
			col.rgb += vec3( (StreetUV.y>1.0-StreetLineWidth && StreetUV.y<1.0)? 1.0 : 0.0);
			col.rgb += vec3( (StreetUV.y>0.5-StreetLineWidth*0.5 && StreetUV.y<0.5+StreetLineWidth*0.5)? 1.0 : 0.0)
				* ( (int(floor(WorldVertexPos.x * 2.0))%2==0)? 1.0 : 0.0 ); // 点線にする
		}

		//col.rgb=vec3(StreetUV,0.0); 
	}
	else if(_IsSidewalk == 1)
	{
		if(g2f_IsZAxis == 1)
		{
			col.rgb = vec3(0.611,0.283,0.211);
		}
		else if(g2f_IsZAxis == 0)
		{
		}
	}

	gl_FragColor=col;
}

)"