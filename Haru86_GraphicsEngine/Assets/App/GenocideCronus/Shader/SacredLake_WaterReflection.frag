R"(

#version 330

in vec2 uv;
in vec3 WorldNormal;
in vec3 WorldVertexPos;
in vec3 CameraPos;

void main(){
	vec3 col=vec3(0.0);
	col=vec3(0.0,0.0,0.0);

	gl_FragColor=vec4(col,1.0);
}

)"