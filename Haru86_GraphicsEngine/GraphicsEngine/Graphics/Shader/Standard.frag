R"(

#version 330

in vec2 uv;
in vec3 CameraPos;
in vec3 WorldVertexPos;
in vec3 WorldNormal;

uniform int _UseColor;
uniform vec4 _Color;
uniform int _UseLighting;
uniform vec3 _LightDir;
uniform vec3 _LightPos;
uniform int _UseEnvColor;
uniform vec4 _EnvColor;

uniform int _UseMainTex;
//uniform sampler2D _MainTex;
uniform samplerCube _MainTex;

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
		vec3 viewdir = normalize(CameraPos-WorldVertexPos);
		vec3 rpdir = normalize(reflect(viewdir,WorldNormal));
		col=texture(_MainTex,rpdir);

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

		vec3 viewDir= -1.0*normalize(WorldVertexPos-CameraPos);
		vec3 halfDir=normalize(viewDir + lightDir);
		float spec=pow( max(0.0,dot(WorldNormal,halfDir)) , 60.0);
		col.rgb+=vec3(1.0)*spec;
	}

	gl_FragColor=col;
}

)"