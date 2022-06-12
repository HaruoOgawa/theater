#version 430

layout(location=0)in vec3 vertex;
layout(location=1)in vec3 normal;
layout(location=2)in vec2 texcoord;


struct BoidsForce{
	vec3 force;
};

layout(std430,binding=0) buffer destBufferBoidsForce
{
	BoidsForce boidsForce[];
} outBufferBoidsForce;

//outBufferBoidsForce.boidsForce[]

struct BoidsData {
    vec3 position;
    vec3 velocity;
	vec3 viewDir;
};

layout(std430,binding=1) buffer destBufferBoidsData
{
	BoidsData boidsData[];
} outBufferBoidsData;

//outBufferBoidsData.boidsData[]

uniform mat4 MVPMatrix;
uniform mat4 MMatrix;
uniform mat4 VMatrix;
uniform mat4 PMatrix;
uniform float _time;
uniform float _deltaTime;
uniform int _boids_count;

#define PI 3.14159265
#define rot(a) mat2(cos(a),sin(a),-sin(a),cos(a))

float atan2(in float y, in float x){
    return x == 0.0 ? sign(y)*PI/2 : atan(y, x);
}

float rand(vec2 seeds){
  return fract(sin(dot(seeds, vec2(12.9898,78.233))) * 43758.5453);
}

out vec2 uv;
out vec3 randCol;
out vec3 v2g_normal;
out float v2g_flag;

mat4 GetRotMatrix(vec3 angle){
               
	/*float sfai=sin(angle.x); float stheta=sin(angle.y); float spusi=sin(angle.z);
	float cfai=cos(angle.x); float ctheta=cos(angle.y); float cpusi=cos(angle.z);
                
	mat4 rotateMatrix=mat4(
		vec4(ctheta*cpusi+stheta*sfai*spusi,-ctheta*spusi+stheta*sfai*spusi,stheta*cfai,0.0),
		vec4(cfai*spusi,cfai*cpusi,-sfai,0.0),
		vec4(-stheta*cpusi+ctheta*sfai*spusi,stheta*spusi+ctheta*sfai*cpusi,ctheta*cfai,0.0),
		vec4(0.0,0.0,0.0,1.0)
    );*/

	mat4 rotateMatrix=
		mat4(
			vec4(cos(angle.z),-sin(angle.z),0.0,0.0),
			vec4(sin(angle.z),cos(angle.z),0.0,0.0),
			vec4(0.0,0.0,1.0,0.0),
			vec4(0.0,0.0,0.0,1.0)
		)*
		mat4(
			vec4(cos(angle.y),0.0,sin(angle.y),0.0),
			vec4(0.0,1.0,0.0,0.0),
			vec4(-sin(angle.y),0.0,cos(angle.y),0.0),
			vec4(0.0,0.0,0.0,1.0)
		)*
		mat4(
			vec4(1.0,0.0,0.0,0.0),
			vec4(0.0,cos(angle.x),-sin(angle.x),0.0),
			vec4(0.0,sin(angle.x),cos(angle.x),0.0),
			vec4(0.0,0.0,0.0,1.0)
		);

    return rotateMatrix;
}

void main(){
	vec3 pos=vec3(rand(vec2(gl_InstanceID,0.123))*2.0-1.0,
		rand(vec2(gl_InstanceID,0.456))*2.0-1.0,
		rand(vec2(gl_InstanceID,0.789))-0.1
	)*300.0;

	int id=gl_InstanceID;
	float flag=1.0;
	if(id>_boids_count/2+1){
		flag=0.0;
	}else{
		flag=1.0;
	}

	bool forceNor =(length(outBufferBoidsForce.boidsForce[id].force)==0.0);
	vec3 now_position=outBufferBoidsData.boidsData[id].position
		-((forceNor)? vec3(0.0) : normalize(outBufferBoidsForce.boidsForce[id].force)*1.5);
	

	vec3 vert=vertex;
	
	//animation VAT
	vert.z+=sin(_time*0.015+gl_InstanceID*100)*pow(length(vert.x),2.0)*1000.0;
	
	
	vert.yz*=rot(-PI/6.0);

	//rotation
    //vec3 force=outBufferBoidsForce.boidsForce[id].force;
    vec3 force=outBufferBoidsData.boidsData[id].viewDir;
    force=(
		(length(force)==0.0)? force : normalize(force)
	);

	//atan2

    float theta_x=PI;
    //theta_x=2.0*PI-theta_x;
    float theta_y=atan2(force.x,force.z);
    float theta_z=atan2(force.x,force.y);

	/*
	float theta_x=atan2(force.z,force.y);
    float theta_y=atan2(force.z,force.x);
    float theta_z=atan2(force.y,force.x);
	*/
      
	  
	//vert.xz*=rot(PI);   
    mat4 rotMat=GetRotMatrix(vec3(theta_x,theta_y,theta_z));
    vert=(rotMat*vec4(vert,1.0)).xyz;
	
	vert*=2000.0;
	pos=(MMatrix*vec4(vert+now_position,1.0)).xyz;

	gl_Position=PMatrix*VMatrix*vec4(pos,1.0);
	
	uv=texcoord;
	randCol=vec3(rand(vec2(gl_InstanceID *100.0,0.987)),
		rand(vec2(gl_InstanceID *100.0,0.654)),
		rand(vec2(gl_InstanceID *100.0,0.321))
	);

	v2g_normal=normal;
	v2g_flag=flag;
}