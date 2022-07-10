R"(

#version 430

in vec2 uv;
in float time;

uniform vec2 _resolution;
uniform sampler2D _SrcTexture;
uniform sampler2D _NormalMap;
uniform float _frameResolusion;

void main(){
	vec3 col=vec3(0.0);
	vec2 st=gl_FragCoord.xy/_resolution.xy;
	
	col=texture(_SrcTexture,st).rgb;
	//col=texture(_NormalMap,st).rgb;
	//col=vec3(st,0.0);

	gl_FragColor=vec4(col,1.0);
}

)"