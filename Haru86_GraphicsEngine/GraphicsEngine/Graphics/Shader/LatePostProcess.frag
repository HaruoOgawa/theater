R"(

#version 410

in vec2 uv;

uniform float _time;
uniform float _deltaTime;
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
uniform int _UseSSR;
uniform int _UseVignette;
uniform float _VignetteRadius;
uniform float _VignetteLateRadius;
uniform float _VignetteBrightness;
uniform int _UseThirdImpact;
uniform int _UseFilmFilter;
uniform int _UseWave;
uniform int _UseRewinding;

float rand(vec2 st)
{
    return fract(sin(dot(st, vec2(12.9898, 78.233))) * 43758.5453);
}

vec3 CalSSRColor(vec3 color){
	vec3 col=color;
	vec2 st=gl_FragCoord.xy/(_resolution.xy *_frameResolusion);
	//vec2 st=uv;
	 
	// OpenGL のデプスは 『0.0(手前)』--> 『1.0(奥)』 なのかい？？？ --> こうだと綺麗だし割と辻褄が合う希ガス
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
	vec4 RawNormalMapColor =texture(_NormalMap,gl_FragCoord.xy/_resolution.xy);
	
	// アルファが0ならノーマルマッピングがオフになっていると見なし抜ける
	float IsUseNormal = RawNormalMapColor.a;
	if(IsUseNormal<=0.0){ return col;}

	// 
	vec3 normal=RawNormalMapColor.rgb*2.0-1.0;

	// ノーマルを使わない場合は『0』としているのでその場合は抜ける --> 主にレイマーチング
	if(length(normal)<=0.0) { return col;}

	//
	vec3 refDir=reflect(viewDir,normal);
	//refDir*=-1.0;  // なんとなく反転 --> なんか見栄えよくなった --> いやただの悪の元凶だったっピ...

	//return refDir;
	//return normal;
	//return abs(normal);
	//return col;
	//return vec3(abs(IsUseNormal));

	// レイで衝突判定をする
	float RAYNUM=100;
	//vec3 step= (2.0/RAYNUM) * refDir;
	vec3 step= (150.0/RAYNUM) * refDir; // (150.0/RAYNUM) --> 1.5倍 --> レイが短すぎて映っていなかったみたい
	float sampledNum=0.0;
	//float objWidth=0.3/RAYNUM;
	//float objWidth=0.3/RAYNUM;
	vec3 ssrColor=vec3(0.0);

	for(float n=0.0;n<RAYNUM;n++)
	{
		vec3 rayPos= pos.xyz + step * n;
		vec4 vpPos = VPMatrix * vec4(rayPos,1.0);
		vec2 rayUV = (vpPos.xy/vpPos.w) * 0.5 +0.5;

		//  rayDepthの定義
		float rayDepth= (vpPos.z/vpPos.w)*0.5+0.5;
		
		float depthOfBuffer = texture(_DepthMapMixed,rayUV*_frameResolusion).r;
		
		// rayDepth < depthOfBuffer --> rayDepthが手前ならそこのピクセル色を塗る
		// 奥ならレイが通りすぎているので塗らない
		if( (rayDepth - depthOfBuffer > 0.0) /*&& (rayDepth - depthOfBuffer < objWidth)*/ )
		//if( (rayDepth < depthOfBuffer) )
		{
			// レイマーチングの場合はこの反転でうまくいく --> でもラスタライザの場合はおかしくなる...(無かったら正常描画)
			//rayUV.y=1.0-rayUV.y;
			
			ssrColor+=texture(_SrcTexture,rayUV*_frameResolusion).rgb;
			//break;
			sampledNum++;
		}
	}

	col=col+ssrColor* (1.0/sampledNum);

	return col;
}

vec3 Vignette(vec3 col)
{
	vec2 st=gl_FragCoord.xy/(_resolution.xy*_frameResolusion);
	st=st*2.0-1.0;
	
	float w = 1.1+(1.0-_VignetteRadius)*4.0 - _VignetteLateRadius*0.5;
	
	float d = (length(st))*w;
	col = mix(col,vec3(0.0),d);
	//col*=d*0.5*(1.0+_VignetteBrightness);
	col*=d*0.5*(1.0+9.0*_VignetteBrightness);

	col.rgb+=(_VignetteBrightness);

	return col;
}

vec3 ThirdImpact(vec3 col)
{
	col = vec3(col.r,col.g*0.1,col.b*0.1);
	col *= 0.75;
	return col;
}

vec3 DrawFilmFilter(vec3 col)
{
	//
	vec2 st=gl_FragCoord.xy/(_resolution.xy*_frameResolusion);

	// ホワイトノイズ
	col *= ( 1.0 - rand(vec2(st.x,st.y)+_time) * (1.0 - min(1.0,abs(sin(_time*0.5))*1.5))  );

	//
	st=st*2.0-1.0;

	float w = 0.2;
	if(abs(st.y) > (1.0-w)) col = vec3(0.0);

	return col;
}

vec3 DrawRewindingFilter(vec3 col)
{
	// ホワイトノイズ
	vec2 st=gl_FragCoord.xy/(_resolution.xy*_frameResolusion);
	col *= rand(vec2(st.x,st.y)+_time);

	// ラインノイズ
	float DomainSize = 100.0;
	float did = rand(vec2(0.971,floor(st.y*DomainSize))) * DomainSize;
	float randStartTime = rand(vec2(did+did,did+0.691+9.99));
	float RandWidth = rand(vec2(did,floor(randStartTime*DomainSize+_time)))*0.1; 
	
	float d0 = abs(st.y) - (mod(_time+randStartTime,1.0));
	float d1 = abs(st.y) - (mod(_time+randStartTime,1.0) - RandWidth);
	if(d0<0.001 && d1>0.001)
	{
		col.rgb *= 0.5;
	}

	// ビデオフィルター
	{
		vec2 vst = st*2.0-1.0;
		float FilterWidth = 0.25;
		if(abs(vst.x) > (1.0-FilterWidth))
		{
			col.rgb = vec3(0.0);
		}
	}

	return col;
}

void main(){
	vec3 col=vec3(0.0);
	vec2 st=gl_FragCoord.xy/_resolution.xy;
	
	if(_UseWave == 1)
	{
		//float val=(sin(_time*2.0)+1.0)*0.5+0.1;
		float val=0.6;
		st.y+=0.005*sin(_time*100.0)*val;
		st.x+=0.001*sin(_time*50.0)*val;
	}
	else if(_UseRewinding == 1)
	{
		st.y = mod(st.y+_time,_frameResolusion);
	}

	col=texture(_SrcTexture,st).rgb;
	
	if(_UseSSR == 1)col=CalSSRColor(col);
	if(_UseVignette == 1) col = Vignette(col);
	if(_UseThirdImpact == 1) col = ThirdImpact(col);
	if(_UseFilmFilter == 1) col = DrawFilmFilter(col);
	if(_UseRewinding == 1) col = DrawRewindingFilter(col);

	gl_FragColor=vec4(col,1.0);
}

)"