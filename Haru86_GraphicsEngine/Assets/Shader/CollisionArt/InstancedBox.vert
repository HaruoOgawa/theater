#version 430

uniform mat4 MVPMatrix;
uniform mat4 MMatrix;
uniform mat4 VMatrix;
uniform mat4 PMatrix;
uniform float _time;
uniform float _deltaTime;
uniform int _instancedCound;
uniform int _sideCubeCount;
uniform float _cubeWidth;

layout(location=0)in vec3 vertex;
layout(location=1)in vec3 normal;
layout(location=2)in vec2 texcoord;

struct SCubeFieldObj
{
    int xIndex;
    int yIndex;
    int zIndex;
};

layout(std430,binding=0) buffer destBufferCubeFieldObj
{
	SCubeFieldObj cubeObj[];
} outBufferCubeFieldObj;

// outBufferCubeFieldObj.cubeObj[]

out vec3 worldPosition;
out vec3 worldNormal;
out float id;
out vec3 randColor;
out vec2 uv;

#define rot(a) mat2(cos(a),-sin(a),sin(a),cos(a))
#define PI 3.14159265

float rand(vec2 seeds){
  return fract(sin(dot(seeds, vec2(12.9898,78.233))) * 43758.5453);
}

vec2 random2(vec2 seeds)
{
 seeds = vec2(dot(seeds, vec2(127.1, 311.7)),
 dot(seeds, vec2(269.5, 183.3)));
 return fract(sin(seeds) * 43758.5453123);
}

float perlinNoise(vec2 seeds)
{
vec2 i = floor(seeds);
vec2 f = fract(seeds);
vec2 i00 = i + vec2(0, 0);
vec2 i10 = i + vec2(1, 0);
vec2 i01 = i + vec2(0, 1);
vec2 i11 = i + vec2(1, 1);
vec2 f00 = f - vec2(0, 0);
vec2 f10 = f - vec2(1, 0);
vec2 f01 = f - vec2(0, 1);
vec2 f11 = f - vec2(1, 1);
vec2 g00 = normalize(-1.0 + 2.0 * random2(i00));
vec2 g10 = normalize(-1.0 + 2.0 * random2(i10));
vec2 g01 = normalize(-1.0 + 2.0* random2(i01));
vec2 g11 = normalize(-1.0 + 2.0* random2(i11));
float v00 = dot(g00, f00);
float v10 = dot(g10, f10);
float v01 = dot(g01, f01);
float v11 = dot(g11, f11);
vec2 p = smoothstep(0.0, 1.0, f);
float v00v10 = mix(v00, v10, p.x);
float v01v11 = mix(v01, v11, p.x);
return mix(v00v10, v01v11, p.y) * 0.5 + 0.5;
}

void main(){
	
	
	mat4x4 ModelMatrix=
		mat4x4(
			vec4(_cubeWidth,0.0,0.0,0.0),
			vec4(0.0,_cubeWidth,0.0,0.0),
			vec4(0.0,0.0,_cubeWidth,0.0),
			vec4(0.0,0.0,0.0,1.0)
		);

	vec3 pos=(ModelMatrix*vec4(vertex,1.0)).xyz;

	SCubeFieldObj objData=outBufferCubeFieldObj.cubeObj[gl_InstanceID];
	float xVal=_cubeWidth*float(_sideCubeCount)*(objData.xIndex/float(_sideCubeCount-1))*2.0-_cubeWidth*float(_sideCubeCount);
	float yVal=_cubeWidth*float(_sideCubeCount)*(objData.yIndex/float(_sideCubeCount-1))*2.0-_cubeWidth*float(_sideCubeCount);
	float zVal=_cubeWidth*float(_sideCubeCount)*(objData.zIndex/float(_sideCubeCount-1))*2.0-_cubeWidth*float(_sideCubeCount);
	
	/*float yVal=-5.0;
	vec2 st=(vec2(float(objData.xIndex),float(objData.yIndex))/float(_sideCubeCount))*2.0-1.0;
	yVal+=perlinNoise(vec2(-_time*0.001,length(st)*100.0))+//*max(1.0,length(st)*2.0)*1.0+
		sin(-_time*0.0025+length(st)*25.0)*max(1.0,length(st)*2.0);*/

	float adjustPos=0.5;
	pos+=vec3(xVal*adjustPos,yVal*adjustPos,zVal*adjustPos);

	gl_Position=PMatrix*VMatrix*vec4(pos,1.0);
	worldPosition=pos;
	worldNormal=normalize((ModelMatrix*vec4(normal,1.0)).xyz);

	id=float(gl_InstanceID);

	randColor=vec3(
		rand(vec2(float(gl_InstanceID),0.159))*2.0,
        rand(vec2(float(gl_InstanceID),7.957))*2.0,
        rand(vec2(float(gl_InstanceID),0.681))*2.0
	);

	uv=texcoord;
}