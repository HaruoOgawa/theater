R"(

#version 330

in vec3 CameraPos;
in vec3 WorldVertexPos;
in vec3 WorldNormal;
in vec3 v2f_billinfo;

uniform int _UseColor;
uniform vec4 _Color;
uniform int _UseLighting;
uniform vec3 _LightDir;
uniform int _UseEnvColor;
uniform vec4 _EnvColor;

void main(){
	vec4 col=vec4(vec3(0.0),1.0);

	// ベースカラー
	if(_UseColor == 1)
	{
		col=_Color;
	}
	else
	{
		col=vec4(1.0);
	}

	// 環境光
	vec4 envColor = vec4(vec3(0.1),1.0);
	if(_UseEnvColor == 1)
	{
		envColor = _EnvColor;
	}

	// ライティング
	if(_UseLighting == 1)
	{
		vec3 lightDir=normalize(_LightDir);
		float diff=max(0.0,dot(WorldNormal,lightDir));
		col.rgb*=diff;

		col.rgb+=envColor.rgb;

		vec3 viewDir= -1.0*normalize(WorldVertexPos-CameraPos);
		vec3 halfDir=normalize(viewDir + lightDir);
		float spec=pow( max(0.0,dot(WorldNormal,halfDir)) , 60.0);
		col.rgb+=vec3(1.0)*spec;
	}

	// 窓
	/*bool IsWindow = (v2f_billinfo.x == 1.0);
	if(IsWindow)
	{
		col.rgb=vec3(0.0,0.0,1.0);
		col.a=0.25;
	}*/


	gl_FragColor=col;
}

)"