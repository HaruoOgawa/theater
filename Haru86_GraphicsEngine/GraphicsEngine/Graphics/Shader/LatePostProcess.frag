R"(

#version 430

in vec2 uv;
in float time;

uniform vec2 _resolution;
uniform sampler2D _SrcTexture;
uniform sampler2D _NormalMap;
uniform sampler2D _DepthMapPolygone;
uniform sampler2D _DepthMapRaymarch;
uniform float _frameResolusion;
uniform mat4 VPMatrix;
uniform mat4 InvVPMatrix;
uniform vec3 _WorldCameraPos;
uniform vec3 _WorldCameraCenter;

vec3 CalSSRColor(vec3 color){
	vec3 col=color;
	vec2 st=gl_FragCoord.xy/_resolution.xy;
	 
	//
	float depth=texture(_DepthMapPolygone,st).r;
	if(depth >= 1.0) return col;

	//vec2 uvpos= (gl_FragCoord.xy*2.0-_resolution.xy)/min(_resolution.x,_resolution.y);
	vec2 uvpos= st*2.0-1.0;
	vec4 pos = InvVPMatrix * vec4(uvpos,depth,1.0);
	pos=pos/pos.w;

	// レイを作成
	vec3 viewDir=-normalize(pos.xyz-_WorldCameraPos);
	vec3 normal=texture(_NormalMap,st).rgb*2.0-1.0;

	// ノーマルを使わない場合は『0』としているのでその場合は抜ける
	if(length(normal)<=0.0) return col;

	//
	vec3 refDir=reflect(viewDir,normal);

	//return pos.xyz;
	return refDir.xyz;
}

void main(){
	vec3 col=vec3(0.0);
	vec2 st=gl_FragCoord.xy/_resolution.xy;
	
	col=texture(_SrcTexture,st).rgb;
	//col=texture(_DepthMapPolygone,st).rgb;
	col=CalSSRColor(col);

	gl_FragColor=vec4(col,1.0);
}

)"