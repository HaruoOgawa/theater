R"(

#version 330

in vec2 uv;

uniform sampler2D _BillWindowTex;
uniform float _frameResolusion;

void main()
{
	vec3 col=vec3(0.0);
	col=vec3(uv,0.0);
	//col=texture(_BillWindowTex,uv).rgb;

	gl_FragColor=vec4(col,1.0);
}

)"