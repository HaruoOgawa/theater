R"(

#version 330

in vec2 uv;
in float time;
in vec2 resolution;

uniform sampler2D _fontTexture;
uniform float _alpha;
uniform float _ColorIndex;

//out vec4 outColor;

void main(){
	vec4 col=vec4(uv.x,uv.y,0.0,1.0);
	col=texture(_fontTexture,1.0-uv);

	if(_ColorIndex==1.0){
		col.rgb=1.0-col.rgb;
	}

	float alpha=col.a;
	col.a=alpha*_alpha;
	gl_FragColor=col;
}

)"