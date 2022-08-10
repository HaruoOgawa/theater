R"(

#version 410

in float g2f_id;
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

uniform samplerCube _BillRP;

void main(){
	// ベースパラメーター
	vec4 col=vec4(vec3(0.0),1.0);
	vec3 viewdir = normalize(_WorldCameraPos-g2f_WorldVertexPos);
	float dist = length(_WorldCameraPos-g2f_WorldVertexPos);
		
	// ベースカラー
	col=vec4(vec3(1.0),1.0);

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
		float diff=max(0.0,dot(g2f_normal,lightDir));
		col.rgb*=diff;

		// Specular
		vec3 viewDir= -1.0*normalize(g2f_WorldVertexPos-_WorldCameraPos);
		vec3 halfDir=normalize(viewDir + lightDir);
		float spec=pow( max(0.0,dot(g2f_normal,halfDir)) , 64.0);
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


		// 反射の方を優先して作る --> ライト方向を変えると意外とライティングいい感じになったから
		vec3 rpdir = normalize(reflect(viewdir,g2f_normal));
		vec3 CubeCol = texture(_BillRP,viewdir).rgb;
		
		col.rgb=mix(CubeCol,col.rgb,clamp(exp(dist*0.2)-1.0,0.0,1.0));
		col.a=1.0;
	}

	// fog
    //vec3 ramda = exp2(-0.05*dist*vec3(1.0));
    //col.rgb=mix(vec3(0.5),col.rgb,ramda);

	gl_FragColor=col;
}

)"