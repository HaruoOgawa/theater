R"(

#version 410

in float g2f_pid;
in vec3 g2f_normal;
in vec4 g2f_billinfo;
in vec3 g2f_WorldVertexPos;

uniform vec3 _CameraPos;
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
		//float diff=max(0.0,dot(g2f_normal,lightDir));
		float diff=dot(g2f_normal,lightDir);
		diff+=0.2;

		col.rgb*=diff;

		col.rgb+=envColor.rgb;

		vec3 viewDir= -1.0*normalize(g2f_WorldVertexPos-_CameraPos);
		vec3 halfDir=normalize(viewDir + lightDir);
		float spec=pow( max(0.0,dot(g2f_normal,halfDir)) , 60.0);
		col.rgb+=vec3(1.0)*spec;
	}

	// 窓
	/*bool IsWindow = (g2f_billinfo.x == 1.0);
	if(IsWindow)
	{
		col.rgb=vec3(0.0,0.0,1.0);
		col.a=0.25;
	}*/

	//col.rgb=g2f_normal;

	gl_FragColor=col;
}

)"