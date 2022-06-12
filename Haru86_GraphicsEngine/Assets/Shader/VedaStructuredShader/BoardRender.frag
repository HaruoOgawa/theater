precision mediump float;

in vec2 uv;
in float time;
in vec2 resolution;

uniform sampler2D frameTex;
uniform float _frameResolusion;
uniform float _postProcessFlag;
uniform float _postProcessVal;

#define PI 3.14159265

float rand(vec2 seeds){
  return fract(sin(dot(seeds, vec2(12.9898,78.233))) * 43758.5453);
}

void main(void){
	vec4 col=vec4(0.,0.,0.,1.);
	
	if(_postProcessFlag==0){
		col.rgb=texture(frameTex,uv*_frameResolusion);
	}
	else if(_postProcessFlag==1){
		float _Strength=1.0;
		int _SampleCount=3;
		vec2 symmetryUv    = uv - 0.5;
		float distance       = length(symmetryUv);
		float factor         = _Strength / _SampleCount * distance;
		for(int j = 0; j < _SampleCount; j++) {
			float uvOffset       = 1 - factor * j;
			col                 += texture(frameTex, (symmetryUv * uvOffset + 0.5)*_frameResolusion);
		}
		col                 /= _SampleCount;
	}else if(_postProcessFlag==2){
				uv.y+=0.05*sin(time*100.0);
				uv.x+=0.01*sin(time*50.0);

				vec4 _GapOffsetX=vec4(1.0,0.5,0.25,0.0);
				vec4 _GapOffsetY=vec4(0.02,0.03,0.03,0.0);
				float _gapOffsetPower=0.08;
				
				vec2 vecFromCenter=vec2(0.0,0.0)-(uv-vec2(0.5,0.5))*2.0;

                vec2 st_R=uv+vecFromCenter*_gapOffsetPower*_GapOffsetX.r;
                vec2 st_G=uv+vecFromCenter*_gapOffsetPower*_GapOffsetX.g;
                vec2 st_B=uv+vecFromCenter*_gapOffsetPower*_GapOffsetX.b;

                col.rgb=texture(frameTex,uv*_frameResolusion);
                col.r+=texture(frameTex,st_R*_frameResolusion);
                col.g+=texture(frameTex,st_G*_frameResolusion);
                col.b+=texture(frameTex,st_B*_frameResolusion);
	}else if(_postProcessFlag==4){
				float val=(sin(time*2.0)+1.0)*0.5+0.1;
				vec2 st=uv;

				uv.y+=0.05*sin(time*100.0)*val;
				uv.x+=0.01*sin(time*50.0)*val;

				vec4 _GapOffsetX=vec4(1.0,0.5,0.25,0.0);
				vec4 _GapOffsetY=vec4(0.02,0.03,0.03,0.0)*6.0;
				float _gapOffsetPower=0.08*val;
				
				vec2 vecFromCenter=vec2(0.0,0.0)-(uv-vec2(0.5,0.5))*2.0;

                vec2 st_R=uv+vecFromCenter*_gapOffsetPower*_GapOffsetX.r;
                vec2 st_G=uv+vecFromCenter*_gapOffsetPower*_GapOffsetX.g;
                vec2 st_B=uv+vecFromCenter*_gapOffsetPower*_GapOffsetX.b;

                col.rgb=texture(frameTex,uv*_frameResolusion);
                col.r+=texture(frameTex,st_R*_frameResolusion);
                col.g+=texture(frameTex,st_G*_frameResolusion);
                col.b+=texture(frameTex,st_B*_frameResolusion);

				col.rgb*=0.5;
				st=2.0*st-1.0;
				float width=0.2;
				col.rgb*=(abs(st.y)>1.0-width)? 0.0:1.0;

	}else if(_postProcessFlag==3){
		col.rgb=texture(frameTex,uv*_frameResolusion);
		col.rgb=1.0-col.rgb;
	}else if(_postProcessFlag==5){
		float glitchY=rand(vec2(0.123,floor(time*25.0)));
		float glitchWidth=0.05;

		if(uv.y>=glitchY&&uv.y<glitchY+glitchWidth){
			uv.x+=sin(time*5.0)*2.0;
		}

		col.rgb=texture(frameTex,uv*_frameResolusion);
		col.rgb*=(uv.x<0.0||uv.x>1.0)? 0.0 : 1.0;

		//noise
		//col.rgb=texture(frameTex,uv*_frameResolusion);
		col.rgb=col.rgb*abs(cos(PI*_postProcessVal))+
			vec3(rand(uv+vec2(time,-1.0*time)))*abs(sin(PI*_postProcessVal));

	}else if(_postProcessFlag==6){
				float val=0.2;
				
				vec2 st=uv;

				uv.y+=0.05*sin(time*100.0)*val;
				uv.x+=0.01*sin(time*50.0)*val;

				vec4 _GapOffsetX=vec4(1.0,0.5,0.25,0.0);
				vec4 _GapOffsetY=vec4(0.02,0.03,0.03,0.0)*6.0;
				float _gapOffsetPower=0.08*val;
				
				vec2 vecFromCenter=vec2(0.0,0.0)-(uv-vec2(0.5,0.5))*2.0;

                vec2 st_R=uv+vecFromCenter*_gapOffsetPower*_GapOffsetX.r;
                vec2 st_G=uv+vecFromCenter*_gapOffsetPower*_GapOffsetX.g;
                vec2 st_B=uv+vecFromCenter*_gapOffsetPower*_GapOffsetX.b;

                col.rgb=texture(frameTex,uv*_frameResolusion);
				col.r+=texture(frameTex,st_R*_frameResolusion);
                col.g+=texture(frameTex,st_G*_frameResolusion);
                col.b+=texture(frameTex,st_B*_frameResolusion);

				col.rgb*=0.5;
				vec4 baseColor=col;

				st=2.0*st-1.0;

				float width=0.2;

				float fadeOutVal=0.2;
				if(_postProcessVal>1.0-fadeOutVal){
					//col.rgb+=max(vec3(0.0),min(vec3(1.0),vec3(1.0)*(1.0-(1.0-_postProcessVal)/fadeOutVal)));
					width=(1.0-_postProcessVal);
				}	
				col.rgb*=(abs(st.y)>1.0-width)? 0.0:1.0;

				

	}
	
	gl_FragColor=col;
}
