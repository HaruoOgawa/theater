R"(

#version 330

in vec2 uv;
in vec3 WorldVertexPos;
in vec3 WorldNormal;

uniform vec3 _WorldCameraPos;
uniform float _time;

float rand(vec2 st)
{
    return fract(sin(dot(st, vec2(12.9898, 78.233))) * 43758.5453);
}


void main(){
	vec4 col=vec4(vec3(0.0),1.0);
	vec2 st = uv * 2.0 - 1.0;
	//col.rgb = vec3(st,0.0);

	float DomainSize = 250.0;
	float did = rand(vec2(0.123,floor(length(st)*DomainSize))) * DomainSize; // ”¼Œa‹——£‚É‰ž‚¶‚½ID

	float randStartTime = rand(vec2(did+did,did+0.691+9.99));
	float randSize = rand(vec2(did,floor(randStartTime*DomainSize+_time))); 
	//vec3 randColor = vec3(1.0,0.75*vec2( rand(vec2(did+1.85+did,did+8.555+floor(randStartTime*DomainSize+_time))) ));
	vec3 randColor = vec3(rand(vec2(did+1.85+did,did+8.555+floor(randStartTime*DomainSize+_time))));

	float d0 = length(st) -(mod(_time+randStartTime,1.0));
	float d1 = length(st) -(mod(_time+randStartTime,1.0) - randSize);
	if(d0<0.001 && d1>0.001)
	{
		col.rgb = randColor;
	}

	gl_FragColor=col;
}

)"