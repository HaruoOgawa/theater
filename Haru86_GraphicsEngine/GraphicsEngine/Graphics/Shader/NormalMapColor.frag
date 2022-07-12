R"(

#version 330

in vec2 uv;
in vec3 WorldNormal;

/*struct g2f{
	vec2 uv;
	vec3 normal;
	float id;
	float billID;
};
in g2f g2f_o; */

out vec4 outColor;

void main(){
	vec3 col=vec3(0.0);
	col=WorldNormal*0.5+0.5;
	//col=g2f_o.normal*0.5+0.5;
	outColor=vec4(col,1.0);
}

)"