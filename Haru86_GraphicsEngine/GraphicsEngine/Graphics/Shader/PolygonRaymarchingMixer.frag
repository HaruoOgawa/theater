R"(

#version 430

uniform float _frameResolusion;

uniform sampler2D polygon_frameTexture;
uniform sampler2D polygon_depthTexture;
uniform sampler2D raymarching_frameTexture;
uniform sampler2D raymarching_depthTexture;

uniform float _existRaymarching;

in vec2 uv;

void main(){
	vec3 col=vec3(uv,1.0);

	//polygon
	vec3 polygonCol=texture(polygon_frameTexture,uv*_frameResolusion).rgb;
	vec3 polygonDepth=texture(polygon_depthTexture,uv*_frameResolusion).rgb;
	//raymarching
	vec3 raymarchingCol=texture(raymarching_frameTexture,uv*_frameResolusion).rgb;
	vec3 raymarchingDepth=texture(raymarching_depthTexture,uv*_frameResolusion).rgb;

	if(_existRaymarching==1.0){
		col=polygonCol;
	}else{
		col=(polygonDepth.r<=raymarchingDepth.r)? polygonCol : raymarchingCol;
	}

	//col=raymarchingCol;
	//col=raymarchingDepth;
	//col=polygonCol;
	//	col=(raymarchingDepth+polygonDepth)*0.5;

	gl_FragColor=vec4(col,1.0);
}


)"