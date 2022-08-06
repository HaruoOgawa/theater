R"(

#version 410

precision highp float;

in vec2 uv;
in float time;
in vec2 resolution;

uniform sampler2D _BlurTexture;
uniform sampler2D _SrcTexture;
uniform float _frameResolusion;

void main(){
	vec3 col=vec3(0.0); 
	col.rgb=texture(_BlurTexture,uv*_frameResolusion).rgb+texture(_SrcTexture,uv*_frameResolusion).rgb;
	//col.rgb=texture(_BlurTexture,uv*_frameResolusion).rgb;
	gl_FragColor=vec4(col,1.0);
}

)"