R"(

#version 410

in vec2 uv;
in vec3 CameraPos;
in vec3 WorldVertexPos;

out vec4 outColor;

void main(){
	vec3 col=vec3(0.0,0.0,0.0);

	float domainSize=100.0;
	vec2 st=fract(uv*domainSize)*2.0-1.0;
	float t=clamp(length(WorldVertexPos-CameraPos)*0.05,0.0,1.0);
	
	float gridWidth=0.05;
	col=vec3(
		((abs(st.x) > (1.0-gridWidth))? 0.5 : 1.0) *
		((abs(st.y) > (1.0-gridWidth))? 0.5 : 1.0) 
	);

	// Œ´“_‚ð•`‚¢‚Ä‚¨‚­
	if(length(uv*2.0-1.0)<0.01)col=vec3(1.0,0.0,0.0);

	// XZŽ²‚ð‘‚¢‚Ä‚¨‚­
	vec2 rootst=uv*2.0-1.0;
	col-=vec3(
		(abs(rootst.y)<0.01)? 1.0 : 0.0,
		0.0,
		(abs(rootst.x)<0.01)? 1.0 : 0.0
	);

	//col*=(1.0-t);

	//gl_FragColor=vec4(col,1.0-t);
	gl_FragColor=vec4(col,0.5);
}

)"