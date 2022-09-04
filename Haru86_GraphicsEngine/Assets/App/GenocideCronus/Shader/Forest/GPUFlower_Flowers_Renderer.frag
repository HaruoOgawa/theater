R"(

#version 330

in vec2 uv;
in vec3 WorldVertexPos;
in vec3 WorldNormal;
flat in float v2f_id;

uniform int _UseLighting;
uniform vec3 _LightDir;
uniform vec3 _LightPos;
uniform vec3 _WorldCameraPos;

// hash --> https://www.shadertoy.com/view/4dffRH
vec3 hash( vec3 p ) // replace this by something better. really. do
{
	p = vec3( dot(p,vec3(127.1,311.7, 74.7)),
			  dot(p,vec3(269.5,183.3,246.1)),
			  dot(p,vec3(113.5,271.9,124.6)));

	return -1.0 + 2.0*fract(sin(p)*43758.5453123);
}

void main(){
	vec4 col=vec4(vec3(1.0),1.0);

	// ベースカラー
	//col.rgb = hash(vec3(v2f_id+4.4412,0.00591,v2f_id))*0.5+0.5;
	//col.rgb = vec3(0.91,0.57,0.6) * 2.0;
	
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

	//col.rgb = WorldNormal*0.5+0.5;

	gl_FragColor=col;
}

)"