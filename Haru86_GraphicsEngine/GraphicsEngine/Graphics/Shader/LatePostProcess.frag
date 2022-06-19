#version 430

in vec2 uv;
in float time;
in vec2 resolution;

uniform sampler2D _SrcTexture;
uniform float _frameResolusion;

void main(){
	vec3 col=vec3(0.0);
	
	col=texture(_SrcTexture,uv*_frameResolusion).rgb;

	gl_FragColor=vec4(col,1.0);
}