R"(

#version 410

in vec3 WorldVertexPos;
in vec3 WorldNormal;
flat in float g2f_id;

uniform int _UseColor;
uniform vec4 _Color;
uniform int _UseLighting;
uniform vec3 _LightPos;
uniform vec3 _WorldCameraPos;
uniform float _time;

uniform int _UseFade;
uniform int _LinearInstanceRate;

void main(){
	vec4 col=vec4(vec3(1.0),1.0);

	// ベースカラー
	//col.rgb = vec3(0.87,0.72,0.53);

	// 環境光
	vec4 envColor = vec4(0.0,0.0,0.0,1.0);
	
	// ライティング
	if(_UseLighting == 1)
	{
		//vec3 lightDir=normalize(_LightDir);
		vec3 lightDir=normalize(_LightPos-WorldVertexPos);
		float diff=max(0.0,dot(WorldNormal,lightDir));
		// とてつもなく、少数部が細かい(桁が多い)数が来るとfloat Textureの精度が足りなくなってMSAA使用時に白いドットのノイズが出てしまうのでその対策
		diff = min(1.0,diff);
		col.rgb*=diff;

		col.rgb+=envColor.rgb;

		vec3 viewDir= -1.0*normalize(WorldVertexPos-_WorldCameraPos);
		vec3 halfDir=normalize(viewDir + lightDir);
		float spec=pow( max(0.0,dot(WorldNormal,halfDir)) , 60.0);
		// とてつもなく、少数部が細かい(桁が多い)数が来るとfloat Textureの精度が足りなくなってMSAA使用時に白いドットのノイズが出てしまうのでその対策
		spec = min(1.0,spec);
		col.rgb+=vec3(1.0)*spec;
	}

	// フェード
	if(_UseFade == 1)
	{
		float nowRate = max(0.0,float(_LinearInstanceRate - 1));
		float nextRate = float(_LinearInstanceRate);

		float NowMaxBillNum = exp2(nowRate);
		float NextMaxBillNum = exp2(nextRate);

		if(g2f_id >= NowMaxBillNum && g2f_id <= NextMaxBillNum)
		{
			float FadeStartTime = 70.0 + float(_LinearInstanceRate - 4) -0.1; // 70スタートらしいのでひとまず直に書いている
			float Alpha = clamp( _time-FadeStartTime ,0.0,1.0);
			col.a = Alpha;
			//col.a = abs(sin(_time));
			//col.a = abs(sin(float(g2f_id)*100.0+_time));
		}
	}

	gl_FragColor=col;
}

)"