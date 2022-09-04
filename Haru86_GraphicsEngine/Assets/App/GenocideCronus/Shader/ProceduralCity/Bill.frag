R"(

#version 410

flat in float g2f_id;
in vec3 g2f_normal;
flat in vec4 g2f_billinfo;
in vec3 g2f_WorldVertexPos;

uniform vec3 _WorldCameraPos;
uniform int _UseColor;
uniform vec4 _Color;
uniform int _UseLighting;
uniform vec3 _LightDir;
uniform vec3 _LightPos;
uniform int _UseEnvColor;
uniform vec4 _EnvColor;
uniform float _time;

uniform samplerCube _BillRP;
uniform int _UseFade;
uniform int _LinearInstanceRate;
uniform int _UseBloom;

 float rand(vec2 st)
{
    return fract(sin(dot(st, vec2(12.9898, 78.233))) * 43758.5453);
}

void main(){
	// ベースパラメーター
	vec4 col=vec4(vec3(0.0),1.0);
	float dist = length(_WorldCameraPos-g2f_WorldVertexPos);
		
	// ベースカラー
	col=vec4(vec3( rand(vec2(g2f_id+1.1111,g2f_id+6.6666))*0.5 +0.5),1.0);
	/*col=vec4(
	vec3(
		rand(vec2(g2f_id+1.1111,g2f_id+6.6666)),
		rand(vec2(g2f_id,g2f_id+0.6232)),
		rand(vec2(g2f_id+g2f_id+9.9999,g2f_id+7.7712))
	),1.0);*/

	// 環境光
	vec4 envColor = vec4(vec3(0.1),1.0);
	if(_UseEnvColor == 1)
	{
		envColor = _EnvColor;
	}

	// ライティング
	if(_UseLighting == 1)
	{
		// diffuse
		vec3 lightDir=normalize(_LightPos-g2f_WorldVertexPos);
		//float diff=max(0.0,dot(g2f_normal,lightDir));
		float diff=max(0.0,abs(dot(g2f_normal,lightDir)));
		col.rgb*=diff;

		// Specular
		vec3 viewDir= -1.0*normalize(g2f_WorldVertexPos-_WorldCameraPos);
		vec3 halfDir=normalize(viewDir + lightDir);
		float spec=pow( max(0.0,dot(g2f_normal,halfDir)) , 64.0);
		// とてつもなく、少数部が細かい(桁が多い)数が来るとfloat Textureの精度が足りなくなってMSAA使用時に白いドットのノイズが出てしまうのでその対策
		spec = min(1.0,spec);
		col.rgb+=vec3(1.0)*spec;

		// Shadow


		// Ambient
		vec3 lightColor = vec3(1.0);
		vec3 ambient = 0.15 * lightColor;
		col.rgb += ambient;
	}

	// 窓
	bool IsWindow = (g2f_billinfo.x == 1.0);
	if(IsWindow)
	{
		// リフレクションの視線ベクトル
		vec3 viewdir = -normalize(_WorldCameraPos-g2f_WorldVertexPos);
		// 反射の方を優先して作る --> ライト方向を変えると意外とライティングいい感じになったから
		vec3 rpdir = normalize(reflect(viewdir,g2f_normal));
		vec3 CubeCol = texture(_BillRP,rpdir).rgb;
		
		col.rgb=mix(CubeCol,col.rgb,clamp(exp(dist*0.2)-1.0,0.0,1.0));
		col.a=1.0;
	}

	// fog
    //vec3 ramda = exp2(-0.05*dist*vec3(1.0));
    //col.rgb=mix(vec3(0.5),col.rgb,ramda);

	// フェード
	if(_UseFade == 1)
	{
		float nowRate = float(10 - _LinearInstanceRate);
		float nextRate = max(0.0,float(10 - _LinearInstanceRate - 1));
		float NowMaxBillNum = exp2(nowRate);
		float NextMaxBillNum = exp2(nextRate);

		if(g2f_id > NextMaxBillNum && g2f_id <= NowMaxBillNum)
		{
			float FadeStartTime = 70.0 + float(_LinearInstanceRate) -0.1; // 70スタートらしいのでひとまず直に書いている
			if(_LinearInstanceRate < 5)
			{
				float Alpha = 1.0 - clamp( _time-FadeStartTime ,0.0,1.0);
				col.a = Alpha;
			}
		}
	}

	if(_UseBloom == 1) col.rgb*=2.0;

	gl_FragColor=col;
}

)"