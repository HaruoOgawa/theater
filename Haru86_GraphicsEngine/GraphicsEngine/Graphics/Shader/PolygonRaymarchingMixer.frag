R"(

#version 410

uniform float _frameResolusion;
uniform vec2 _resolution;

uniform sampler2D polygon_frameTexture;
uniform sampler2D polygon_depthTexture;
uniform sampler2D polygon_normalTexture;
uniform sampler2D raymarching_frameTexture;
uniform sampler2D raymarching_depthTexture;

uniform float _existRaymarching;
uniform float _time;
uniform float _IsDepthMix;

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

	if(_IsDepthMix==1.0){
		col=(polygonDepth+raymarchingDepth)*0.5;
	}

	//col=raymarchingCol;
	//col=raymarchingDepth;
	//col=polygonNormal;
	//col=polygonCol;
	//col=polygonDepth;

	gl_FragColor=vec4(col,1.0);
}


)"