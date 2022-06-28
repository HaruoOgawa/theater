R"(

#version 430

uniform sampler2D _BillWindowTex;
uniform float _frameResolusion;

struct g2f{
	vec2 uv;
};

in g2f g2f_o; 

void main()
{
	vec3 col=vec3(0.0);
	vec2 st=g2f_o.uv;
	col=vec3(st,0.0);
	//col=texture(_BillWindowTex,st).rgb;
	//col.r=1.0;

	gl_FragColor=vec4(col,1.0);
}

)"