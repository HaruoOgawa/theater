R"(

#version 410

in vec2 uv;
in float time;
in vec2 resolution;

uniform sampler2D _BloomTexture;
uniform sampler2D _SrcTexture;
uniform float _UseBloom;
uniform float _frameResolusion;

void main(){
	vec3 col=vec3(0.0); 
	if(_UseBloom==1.0){
		col.rgb=texture(_BloomTexture,uv*_frameResolusion).rgb;
	}else{
		col.rgb=texture(_SrcTexture,uv*_frameResolusion).rgb;
	}

	gl_FragColor=vec4(col,1.0);
}

)"