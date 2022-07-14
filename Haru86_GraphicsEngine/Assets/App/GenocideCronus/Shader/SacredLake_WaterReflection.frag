R"(

#version 410

in vec3 g2f_normal;

void main(){
	vec3 col=vec3(0.0);
	//col=vec3(0.0,0.0,0.0);
	col=g2f_normal;

	gl_FragColor=vec4(col,1.0);
}

)"