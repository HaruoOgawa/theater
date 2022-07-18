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
		//col=vec4(uv.x,uv.y,0.0,1.0);
	}

	// ライティング
	if(_UseLighting == 1)
	{
		vec3 lightDir=normalize(_LightDir);
		float diff=max(0.0,dot(WorldNormal,lightDir));
		col.rgb*=diff;

		vec3 viewDir= -1.0*normalize(WorldVertexPos-CameraPos);
		vec3 halfDir=normalize(viewDir + lightDir);
		float spec=pow( max(0.0,dot(WorldNormal,halfDir)) , 60.0);
		col.rgb+=vec3(1.0)*spec;
	}

	gl_FragColor=col;
}

)"