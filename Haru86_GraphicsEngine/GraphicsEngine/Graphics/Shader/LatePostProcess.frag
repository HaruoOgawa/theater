R"(

#version 430

in vec2 uv;
in float time;

uniform vec2 _resolution;
uniform sampler2D _SrcTexture;
uniform sampler2D _NormalMap;
uniform sampler2D _DepthMapPolygone;
uniform sampler2D _DepthMapRaymarch;
uniform sampler2D _DepthMapMixed;
uniform float _frameResolusion;
uniform mat4 VPMatrix;
uniform mat4 InvVPMatrix;
uniform vec3 _WorldCameraPos;
uniform vec3 _WorldCameraCenter;

vec3 CalSSRColor(vec3 color){
	vec3 col=color;
	vec2 st=gl_FragCoord.xy/(_resolution.xy *_frameResolusion);
	//vec2 st=uv;
	 
	// OpenGL のデプスは 『0.0(手前)』--> 『1.0(奥)』 なのかい？？？
	// http://www.opengl-tutorial.org/ru/intermediate-tutorials/tutorial-14-render-to-texture/

	float depth=texture(_DepthMapPolygone,gl_FragCoord.xy/_resolution.xy).r;
	if(depth >= 1.0) return col;

	//vec2 uvpos= (gl_FragCoord.xy*2.0-_resolution.xy)/min(_resolution.x,_resolution.y);
	vec2 uvpos= st*2.0-1.0;
	vec4 pos = InvVPMatrix * vec4(uvpos,depth,1.0);
	pos=pos/pos.w;

	//return vec3(st,0.0);
	//return vec3(uvpos,0.0);
	//return pos.xyz;
	//return vec3(depth);

	// レイを作成
	vec3 viewDir=-normalize(pos.xyz-_WorldCameraPos);
	//vec3 normal=texture(_NormalMap,st).rgb*2.0-1.0;
	vec3 normal=texture(_NormalMap,gl_FragCoord.xy/_resolution.xy).rgb*2.0-1.0;

	// ノーマルを使わない場合は『0』としているのでその場合は抜ける
	if(length(normal)<=0.0) return col;

	//
	vec3 refDir=reflect(viewDir,normal);
	refDir*=-1.0;  // なんとなく反転 --> なんか見栄えよくなった

	//return refDir;
	//return normal;

	// レイで衝突判定をする
	float RAYNUM=100;
	vec3 step= (2.0/RAYNUM) * refDir;
	float sampledNum=0.0;
	//float objWidth=0.3/RAYNUM;
	float objWidth=0.3/RAYNUM;
	vec3 ssrColor=vec3(0.0);

	for(float n=0.0;n<RAYNUM;n++)
	{
		vec3 rayPos= pos.xyz + step * n;
		vec4 vpPos = VPMatrix * vec4(rayPos,1.0);
		vec2 rayUV = (vpPos.xy/vpPos.w) * 0.5 +0.5;

		//  rayDepthの定義、たぶん間違っている
		float rayDepth= (vpPos.z/vpPos.w)*0.5+0.5;
		
		float depthOfBuffer = texture(_DepthMapMixed,rayUV*_frameResolusion).r;
		
		// rayDepth < depthOfBuffer --> rayDepthが手前ならそこのピクセル色を塗る
		// 奥ならレイが通りすぎているので塗らない
		if( (rayDepth - depthOfBuffer > 0.0) /*&& (rayDepth - depthOfBuffer < objWidth)*/ )
		//if( (rayDepth < depthOfBuffer) )
		{
			// レイマーチングの場合はこの反転でうまくいく --> でもラスタライザの場合はおかしくなる...(無かったら正常描画)
			rayUV.y=1.0-rayUV.y;
			
			ssrColor+=texture(_SrcTexture,rayUV*_frameResolusion).rgb * 0.4;
			//break;
			sampledNum++;
		}
	}

	col=col+ssrColor* (1.0/sampledNum);

	return col;
}

void main(){
	vec3 col=vec3(0.0);
	vec2 st=gl_FragCoord.xy/_resolution.xy;
	
	col=texture(_SrcTexture,st).rgb;
	//col=texture(_DepthMapPolygone,st).rgb;
	//col=texture(_DepthMapMixed,st).rgb;
	col=CalSSRColor(col);

	gl_FragColor=vec4(col,1.0);
}

)"