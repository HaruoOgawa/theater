R"(

#version 410

in vec2 uv;

void main(){
	vec3 col=vec3(1.0,0.0,0.0);

	col+=vec3(1.0);

	gl_FragColor=vec4(col,1.0);
}

)"