precision mediump float;

in vec2 uv;
in float time;
in vec2 resolution;

uniform sampler2D frameTex;
uniform float _frameResolusion;

void main(void){
	vec4 col=vec4(0.,0.,0.,1.);
	col.rgb=texture(frameTex,uv*_frameResolusion);
	gl_FragColor=col;
}
