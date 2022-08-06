R"(

#version 410

precision highp float;

in vec2 uv;
in float time;
in vec2 resolution;

uniform sampler2D _SrcTexture;
uniform float _frameResolusion;
uniform float _BloomThreshold;
uniform float _BloomIntensity;

void main(){
	vec3 col=vec3(0.0); 
	
	col=texture(_SrcTexture,uv*_frameResolusion).rgb;

	// ‹P“x‚ðŒvŽZ 1
	col.rgb=max(col.rgb-_BloomThreshold,0.0)*_BloomIntensity;

	// ‹P“x‚ðŒvŽZ 2
	/*float brightness=dot(col,vec3(0.2126, 0.7152, 0.0722));
	if(brightness<=1.0){
		col=vec3(0.0);
	}*/

	gl_FragColor=vec4(col,1.0);
}

)"