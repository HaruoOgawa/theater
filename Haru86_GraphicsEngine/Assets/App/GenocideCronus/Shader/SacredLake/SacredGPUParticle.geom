R"(

#version 410

layout(points) in;
layout(triangle_strip,max_vertices=256) out;

uniform mat4 MVPMatrix;
uniform mat4 MMatrix;
uniform mat4 VMatrix;
uniform mat4 PMatrix;

struct v2g{
	vec4 position;
	vec2 uv;
	mat4 RandPosMat;
	mat4 LocalMatrix;
	int id;
};

struct g2f{
	vec2 uv;
	float id;
	float billID;
};

in v2g v2g_o[]; 

out g2f g2f_o; 
out vec3 WorldNormal;

float rand(vec2 seeds){
  return fract(sin(dot(seeds, vec2(12.9898,78.233))) * 43758.5453);
}

void CreateCube(vec4 position,vec3 size){
	// +x
	gl_Position=MVPMatrix*vec4(vec3(position.xyz+(v2g_o[0].LocalMatrix*vec4(0.5*size.x,0.0,-0.5*size.z,1.0)).xyz),1.0);
	g2f_o.uv=vec2(0.0,0.0);
	WorldNormal=(v2g_o[0].LocalMatrix*vec4(normalize(vec3(1.0,0.0,0.0)),1.0)).xyz; 
	g2f_o.id=float(v2g_o[0].id); g2f_o.billID=1.0; EmitVertex(); 
	
	gl_Position=MVPMatrix*vec4(vec3(position.xyz+(v2g_o[0].LocalMatrix*vec4(0.5*size.x,0.0,0.5*size.z,1.0)).xyz),1.0);
	g2f_o.uv=vec2(1.0,0.0);
	WorldNormal=(v2g_o[0].LocalMatrix*vec4(normalize(vec3(1.0,0.0,0.0)),1.0)).xyz;
	g2f_o.id=float(v2g_o[0].id); g2f_o.billID=1.0; EmitVertex(); 

	gl_Position=MVPMatrix*vec4(vec3(position.xyz+(v2g_o[0].LocalMatrix*vec4(0.5*size.x,size.y,-0.5*size.z,1.0)).xyz),1.0);
	g2f_o.uv=vec2(0.0,1.0);
	WorldNormal=(v2g_o[0].LocalMatrix*vec4(normalize(vec3(1.0,0.0,0.0)),1.0)).xyz;
	g2f_o.id=float(v2g_o[0].id); g2f_o.billID=1.0; EmitVertex(); 
	
	gl_Position=MVPMatrix*vec4(vec3(position.xyz+(v2g_o[0].LocalMatrix*vec4(0.5*size.x,size.y,0.5*size.z,1.0)).xyz),1.0);
	g2f_o.uv=vec2(1.0,1.0);
	WorldNormal=(v2g_o[0].LocalMatrix*vec4(normalize(vec3(1.0,0.0,0.0)),1.0)).xyz;
	g2f_o.id=float(v2g_o[0].id); g2f_o.billID=1.0; EmitVertex(); 

	EndPrimitive();
	
	// -x
	gl_Position=MVPMatrix*vec4(vec3(position.xyz+(v2g_o[0].LocalMatrix*vec4(-0.5*size.x,0.0,-0.5*size.z,1.0)).xyz),1.0);
	g2f_o.uv=vec2(0.0,0.0);
	WorldNormal=(v2g_o[0].LocalMatrix*vec4(normalize(vec3(-1.0,0.0,0.0)),1.0)).xyz;
	g2f_o.id=float(v2g_o[0].id); g2f_o.billID=1.0; EmitVertex(); 

	gl_Position=MVPMatrix*vec4(vec3(position.xyz+(v2g_o[0].LocalMatrix*vec4(-0.5*size.x,0.0,0.5*size.z,1.0)).xyz),1.0);
	g2f_o.uv=vec2(1.0,0.0);
	WorldNormal=(v2g_o[0].LocalMatrix*vec4(normalize(vec3(-1.0,0.0,0.0)),1.0)).xyz;
	g2f_o.id=float(v2g_o[0].id); g2f_o.billID=1.0; EmitVertex(); 

	gl_Position=MVPMatrix*vec4(vec3(position.xyz+(v2g_o[0].LocalMatrix*vec4(-0.5*size.x,size.y,-0.5*size.z,1.0)).xyz),1.0);
	g2f_o.uv=vec2(0.0,1.0);
	WorldNormal=(v2g_o[0].LocalMatrix*vec4(normalize(vec3(-1.0,0.0,0.0)),1.0)).xyz;
	g2f_o.id=float(v2g_o[0].id); g2f_o.billID=1.0; EmitVertex(); 
	
	gl_Position=MVPMatrix*vec4(vec3(position.xyz+(v2g_o[0].LocalMatrix*vec4(-0.5*size.x,size.y,0.5*size.z,1.0)).xyz),1.0);
	g2f_o.uv=vec2(1.0,1.0);
	WorldNormal=(v2g_o[0].LocalMatrix*vec4(normalize(vec3(-1.0,0.0,0.0)),1.0)).xyz;
	g2f_o.id=float(v2g_o[0].id); g2f_o.billID=1.0; EmitVertex(); 

	EndPrimitive();

	// +y
	gl_Position=MVPMatrix*vec4(vec3(position.xyz+(v2g_o[0].LocalMatrix*vec4(-0.5*size.x,size.y,-0.5*size.z,1.0)).xyz),1.0);
	g2f_o.uv=vec2(0.0,0.0);
	WorldNormal=(v2g_o[0].LocalMatrix*vec4(normalize(vec3(0.0,1.0,0.0)),1.0)).xyz;
	g2f_o.id=float(v2g_o[0].id); g2f_o.billID=1.0; EmitVertex(); 

	gl_Position=MVPMatrix*vec4(vec3(position.xyz+(v2g_o[0].LocalMatrix*vec4(0.5*size.x,size.y,-0.5*size.z,1.0)).xyz),1.0);
	g2f_o.uv=vec2(1.0,0.0);
	WorldNormal=(v2g_o[0].LocalMatrix*vec4(normalize(vec3(0.0,1.0,0.0)),1.0)).xyz;
	g2f_o.id=float(v2g_o[0].id); g2f_o.billID=1.0; EmitVertex(); 

	gl_Position=MVPMatrix*vec4(vec3(position.xyz+(v2g_o[0].LocalMatrix*vec4(-0.5*size.x,size.y,0.5*size.z,1.0)).xyz),1.0);
	g2f_o.uv=vec2(0.0,1.0);
	WorldNormal=(v2g_o[0].LocalMatrix*vec4(normalize(vec3(0.0,1.0,0.0)),1.0)).xyz;
	g2f_o.id=float(v2g_o[0].id); g2f_o.billID=1.0; EmitVertex(); 
	
	gl_Position=MVPMatrix*vec4(vec3(position.xyz+(v2g_o[0].LocalMatrix*vec4(0.5*size.x,size.y,0.5*size.z,1.0)).xyz),1.0);
	g2f_o.uv=vec2(1.0,1.0);
	WorldNormal=(v2g_o[0].LocalMatrix*vec4(normalize(vec3(0.0,1.0,0.0)),1.0)).xyz;
	g2f_o.id=float(v2g_o[0].id); g2f_o.billID=1.0; EmitVertex(); 

	EndPrimitive();
	
	// -y
	gl_Position=MVPMatrix*vec4(vec3(position.xyz+(v2g_o[0].LocalMatrix*vec4(-0.5*size.x,0.0,-0.5*size.z,1.0)).xyz),1.0);
	g2f_o.uv=vec2(0.0,0.0);
	WorldNormal=(v2g_o[0].LocalMatrix*vec4(normalize(vec3(0.0,-1.0,0.0)),1.0)).xyz;
	g2f_o.id=float(v2g_o[0].id); g2f_o.billID=1.0; EmitVertex(); 

	gl_Position=MVPMatrix*vec4(vec3(position.xyz+(v2g_o[0].LocalMatrix*vec4(0.5*size.x,0.0,-0.5*size.z,1.0)).xyz),1.0);
	g2f_o.uv=vec2(1.0,0.0);
	WorldNormal=(v2g_o[0].LocalMatrix*vec4(normalize(vec3(0.0,-1.0,0.0)),1.0)).xyz;
	g2f_o.id=float(v2g_o[0].id); g2f_o.billID=1.0; EmitVertex(); 

	gl_Position=MVPMatrix*vec4(vec3(position.xyz+(v2g_o[0].LocalMatrix*vec4(-0.5*size.x,0.0,0.5*size.z,1.0)).xyz),1.0);
	g2f_o.uv=vec2(0.0,1.0);
	WorldNormal=(v2g_o[0].LocalMatrix*vec4(normalize(vec3(0.0,-1.0,0.0)),1.0)).xyz;
	g2f_o.id=float(v2g_o[0].id); g2f_o.billID=1.0; EmitVertex(); 
	
	gl_Position=MVPMatrix*vec4(vec3(position.xyz+(v2g_o[0].LocalMatrix*vec4(0.5*size.x,0.0,0.5*size.z,1.0)).xyz),1.0);
	g2f_o.uv=vec2(1.0,1.0);
	WorldNormal=(v2g_o[0].LocalMatrix*vec4(normalize(vec3(0.0,-1.0,0.0)),1.0)).xyz;
	g2f_o.id=float(v2g_o[0].id); g2f_o.billID=1.0; EmitVertex(); 

	EndPrimitive();

	// +z
	gl_Position=MVPMatrix*vec4(vec3( position.xyz+ (v2g_o[0].LocalMatrix*vec4(-0.5*size.x,0.0,0.5*size.z,1.0)).xyz ),1.0);
	g2f_o.uv=vec2(0.0,0.0);
	WorldNormal=(v2g_o[0].LocalMatrix*vec4(normalize(vec3(0.0,0.0,1.0)),1.0)).xyz;
	g2f_o.id=float(v2g_o[0].id); g2f_o.billID=1.0; EmitVertex(); 

	gl_Position=MVPMatrix*vec4(vec3(position.xyz+(v2g_o[0].LocalMatrix*vec4(0.5*size.x,0.0,0.5*size.z,1.0)).xyz),1.0);
	g2f_o.uv=vec2(1.0,0.0);
	WorldNormal=(v2g_o[0].LocalMatrix*vec4(normalize(vec3(0.0,0.0,1.0)),1.0)).xyz;
	g2f_o.id=float(v2g_o[0].id); g2f_o.billID=1.0; EmitVertex(); 

	gl_Position=MVPMatrix*vec4(vec3(position.xyz+(v2g_o[0].LocalMatrix*vec4(-0.5*size.x,size.y,0.5*size.z,1.0)).xyz),1.0);
	g2f_o.uv=vec2(0.0,1.0);
	WorldNormal=(v2g_o[0].LocalMatrix*vec4(normalize(vec3(0.0,0.0,1.0)),1.0)).xyz;
	g2f_o.id=float(v2g_o[0].id); g2f_o.billID=1.0; EmitVertex(); 
	
	gl_Position=MVPMatrix*vec4(vec3(position.xyz+(v2g_o[0].LocalMatrix*vec4(0.5*size.x,size.y,0.5*size.z,1.0)).xyz),1.0);
	g2f_o.uv=vec2(1.0,1.0);
	WorldNormal=(v2g_o[0].LocalMatrix*vec4(normalize(vec3(0.0,0.0,1.0)),1.0)).xyz;
	g2f_o.id=float(v2g_o[0].id); g2f_o.billID=1.0; EmitVertex(); 

	EndPrimitive();
	
	// -z
	gl_Position=MVPMatrix*vec4(vec3(position.xyz+(v2g_o[0].LocalMatrix*vec4(-0.5*size.x,0.0,-0.5*size.z,1.0)).xyz),1.0);
	g2f_o.uv=vec2(0.0,0.0);
	WorldNormal=(v2g_o[0].LocalMatrix*vec4(normalize(vec3(0.0,0.0,-1.0)),1.0)).xyz;
	g2f_o.id=float(v2g_o[0].id); g2f_o.billID=1.0; EmitVertex(); 

	gl_Position=MVPMatrix*vec4(vec3(position.xyz+(v2g_o[0].LocalMatrix*vec4(0.5*size.x,0.0,-0.5*size.z,1.0)).xyz),1.0);
	g2f_o.uv=vec2(1.0,0.0);
	WorldNormal=(v2g_o[0].LocalMatrix*vec4(normalize(vec3(0.0,0.0,-1.0)),1.0)).xyz;
	g2f_o.id=float(v2g_o[0].id); g2f_o.billID=1.0; EmitVertex(); 

	gl_Position=MVPMatrix*vec4(vec3(position.xyz+(v2g_o[0].LocalMatrix*vec4(-0.5*size.x,size.y,-0.5*size.z,1.0)).xyz),1.0);
	g2f_o.uv=vec2(0.0,1.0);
	WorldNormal=(v2g_o[0].LocalMatrix*vec4(normalize(vec3(0.0,0.0,-1.0)),1.0)).xyz;
	g2f_o.id=float(v2g_o[0].id); g2f_o.billID=1.0; EmitVertex(); 
	
	gl_Position=MVPMatrix*vec4(vec3(position.xyz+(v2g_o[0].LocalMatrix*vec4(0.5*size.x,size.y,-0.5*size.z,1.0)).xyz),1.0);
	g2f_o.uv=vec2(1.0,1.0);
	WorldNormal=(v2g_o[0].LocalMatrix*vec4(normalize(vec3(0.0,0.0,-1.0)),1.0)).xyz;
	g2f_o.id=float(v2g_o[0].id); g2f_o.billID=1.0; EmitVertex(); 

	EndPrimitive();
}

void main()
{
	CreateCube(v2g_o[0].RandPosMat*gl_in[0].gl_Position,vec3(1.0));
	//CreateCube(gl_in[0].gl_Position+vec4(v2g_o[0].randPos,0.0),vec3(1.0));
}

)"