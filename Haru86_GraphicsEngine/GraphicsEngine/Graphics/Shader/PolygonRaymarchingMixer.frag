R"(

#version 430

uniform float _frameResolusion;
uniform vec2 _resolution;

uniform sampler2D polygon_frameTexture;
uniform sampler2D polygon_depthTexture;
uniform sampler2D polygon_normalTexture;
uniform sampler2D raymarching_frameTexture;
uniform sampler2D raymarching_depthTexture;

uniform float _existRaymarching;
uniform float _time;

in vec2 uv;

void main(){
	vec3 col=vec3(uv,1.0);
	vec2 st=gl_FragCoord.xy/_resolution.xy;

	//polygon
	vec3 polygonCol=texture(polygon_frameTexture,st).rgb;
	vec3 polygonDepth=texture(polygon_depthTexture,st).rgb;
	vec3 polygonNormal=texture(polygon_normalTexture,st).rgb;
	//raymarching
	vec3 raymarchingCol=texture(raymarching_frameTexture,st).rgb;
	vec3 raymarchingDepth=texture(raymarching_depthTexture,st).rgb;

	if(_existRaymarching==1.0){
		col=polygonCol;
	}else{
		col=(polygonDepth.r<=raymarchingDepth.r)? polygonCol : raymarchingCol;
	}

	//col=raymarchingCol;
	//col=raymarchingDepth;
	col=polygonNormal;
	//col=polygonCol;
	//col=polygonDepth;
	//col=(raymarchingCol+polygonCol)*0.5;
	//col=(raymarchingDepth+polygonDepth)*0.5;
	//col=mix( (raymarchingCol+polygonCol)*0.5 , (raymarchingDepth+polygonDepth)*0.5 ,abs(sin(_time)));
    
	gl_FragColor=vec4(col,1.0);
}


)"