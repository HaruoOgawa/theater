precision mediump float;

in vec2 uv;
in float time;
in vec2 resolution;

uniform sampler2D frameTex;
uniform float _frameResolusion;

void main(void){
	vec4 col=texture(frameTex,uv*_frameResolusion);
	col.rgb=1.0-col.rgb;
	gl_FragColor=col;
}