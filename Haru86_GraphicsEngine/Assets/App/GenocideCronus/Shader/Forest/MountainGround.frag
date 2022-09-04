R"(

#version 330

in vec2 uv;
in vec3 WorldVertexPos;
in vec3 WorldNormal;

uniform int _UseColor;
uniform vec4 _Color;
uniform int _UseLighting;
uniform vec3 _LightDir;
uniform vec3 _LightPos;
uniform vec3 _WorldCameraPos;
uniform int _UseEnvColor;
uniform vec4 _EnvColor;

float hash(vec3 p)
{
    p=50.0*fract( p*0.3183099 + vec3(0.71,0.113,0.419));
    return -1.0+2.0*fract( p.x*p.y*p.z*(p.x+p.y+p.z) );
}

vec4 noised(in vec3 x)
{
    vec3 p=floor(x);
    vec3 w=fract(x);
    
    vec3 u=w*w*w*(w*(w*6.0-15.0)+10.0);
    vec3 du=30.0*w*w*(w*(w-2.0)+1.0);
    
    float a = hash( p+vec3(0,0,0) );
    float b = hash( p+vec3(1,0,0) );
    float c = hash( p+vec3(0,1,0) );
    float d = hash( p+vec3(1,1,0) );
    float e = hash( p+vec3(0,0,1) );
    float f = hash( p+vec3(1,0,1) );
    float g = hash( p+vec3(0,1,1) );
    float h = hash( p+vec3(1,1,1) );
    
    float k0 = a;
    float k1 = b-a;
    float k2 = c-a;
    float k3 = e-a;
    float k4 = a-b-c+d;
    float k5 = a-c-e+g;
    float k6 = a-b-e+f;
    float k7 =-a+b+c-d+e-f-g+h;
    
    return vec4( -1.0+2.0*(k0 + k1*u.x + k2*u.y + k3*u.z + k4*u.x*u.y + k5*u.y*u.z + k6*u.z*u.x + k7*u.x*u.y*u.z),
                 2.0* du * vec3( k1 + k4*u.y + k6*u.z + k7*u.y*u.z,
                                 k2 + k5*u.z + k4*u.x + k7*u.z*u.x,
                                 k3 + k6*u.x + k5*u.y + k7*u.x*u.y ) );
}


void main(){
	vec4 col=vec4(vec3(0.0),1.0);

	// ベースカラー
	/*if(_UseColor == 1)
	{
		col=_Color;
	}
	else*/
	{
		col=vec4(1.0);
	}

	

	// 環境光
	vec4 envColor = vec4(0.0,0.0,0.0,1.0);
	if(_UseEnvColor == 1)
	{
		envColor = _EnvColor;
	}

	// ライティング
	if(_UseLighting == 1)
	{
		//vec3 lightDir=normalize(_LightDir);
		vec3 lightDir=normalize(_LightPos-WorldVertexPos);
		float diff=max(0.0,dot(WorldNormal,lightDir));
		col.rgb*=diff;

		col.rgb+=envColor.rgb;

		vec3 viewDir= -1.0*normalize(WorldVertexPos-_WorldCameraPos);
		vec3 halfDir=normalize(viewDir + lightDir);
		float spec=pow( max(0.0,dot(WorldNormal,halfDir)) , 60.0);
		// とてつもなく、少数部が細かい(桁が多い)数が来るとfloat Textureの精度が足りなくなってMSAA使用時に白いドットのノイズが出てしまうのでその対策
		spec = min(1.0,spec);
		col.rgb+=vec3(1.0)*spec;

		//col.rgb=vec3(1.0);
		col.rgb+=(noised(vec3(viewDir*1000.0))*0.5+0.5).r*0.05;
	}

	gl_FragColor=col;
}

)"