R"(

#version 330

in vec2 uv;

//out vec4 outColor;

uniform int _UseColor;
uniform vec4 _Color;

void main(){
	if(_UseColor == 1)
	{
		gl_FragColor=_Color;
	}
	else
	{
		gl_FragColor=vec4(uv.x,uv.y,0.0,1.0);
	}
}

)"