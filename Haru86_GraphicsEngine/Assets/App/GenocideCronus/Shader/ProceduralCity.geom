R"(

#version 430

layout(points) in;
layout(triangle_strip,max_vertices=256) out;

uniform mat4 MVPMatrix;
uniform mat4 MMatrix;
uniform mat4 VMatrix;
uniform mat4 PMatrix;

struct v2g{
	vec4 position;
	vec2 uv;
	vec3 randPos;
	int id;
};

struct g2f{
	vec2 uv;
	vec3 normal;
};

in v2g v2g_o[]; 

out g2f g2f_o; 

float rand(vec2 seeds){
  return fract(sin(dot(seeds, vec2(12.9898,78.233))) * 43758.5453);
}

void CreateCube(vec4 position,vec3 size){
	// +x
	gl_Position=MVPMatrix*vec4(vec3(position.xyz+(vec3(0.5*size.x,0.0,-0.5*size.z))),1.0);
	g2f_o.uv=vec2(0.0,0.0);
	g2f_o.normal=normalize(vec3(1.0,0.0,0.0));
	EmitVertex();

	gl_Position=MVPMatrix*vec4(vec3(position.xyz+(vec3(0.5*size.x,0.0,0.5*size.z))),1.0);
	g2f_o.uv=vec2(1.0,0.0);
	g2f_o.normal=normalize(vec3(1.0,0.0,0.0));
	EmitVertex();

	gl_Position=MVPMatrix*vec4(vec3(position.xyz+(vec3(0.5*size.x,size.y,-0.5*size.z))),1.0);
	g2f_o.uv=vec2(0.0,1.0);
	g2f_o.normal=normalize(vec3(1.0,0.0,0.0));
	EmitVertex();
	
	gl_Position=MVPMatrix*vec4(vec3(position.xyz+(vec3(0.5*size.x,size.y,0.5*size.z))),1.0);
	g2f_o.uv=vec2(1.0,1.0);
	g2f_o.normal=normalize(vec3(1.0,0.0,0.0));
	EmitVertex();

	EndPrimitive();
	
	// -x
	gl_Position=MVPMatrix*vec4(vec3(position.xyz+(vec3(-0.5*size.x,0.0,-0.5*size.z))),1.0);
	g2f_o.uv=vec2(0.0,0.0);
	g2f_o.normal=normalize(vec3(-1.0,0.0,0.0));
	EmitVertex();

	gl_Position=MVPMatrix*vec4(vec3(position.xyz+(vec3(-0.5*size.x,0.0,0.5*size.z))),1.0);
	g2f_o.uv=vec2(1.0,0.0);
	g2f_o.normal=normalize(vec3(-1.0,0.0,0.0));
	EmitVertex();

	gl_Position=MVPMatrix*vec4(vec3(position.xyz+(vec3(-0.5*size.x,size.y,-0.5*size.z))),1.0);
	g2f_o.uv=vec2(0.0,1.0);
	g2f_o.normal=normalize(vec3(-1.0,0.0,0.0));
	EmitVertex();
	
	gl_Position=MVPMatrix*vec4(vec3(position.xyz+(vec3(-0.5*size.x,size.y,0.5*size.z))),1.0);
	g2f_o.uv=vec2(1.0,1.0);
	g2f_o.normal=normalize(vec3(-1.0,0.0,0.0));
	EmitVertex();

	EndPrimitive();

	// +y
	gl_Position=MVPMatrix*vec4(vec3(position.xyz+(vec3(-0.5*size.x,size.y,-0.5*size.z))),1.0);
	g2f_o.uv=vec2(0.0,0.0);
	g2f_o.normal=normalize(vec3(0.0,1.0,0.0));
	EmitVertex();

	gl_Position=MVPMatrix*vec4(vec3(position.xyz+(vec3(0.5*size.x,size.y,-0.5*size.z))),1.0);
	g2f_o.uv=vec2(1.0,0.0);
	g2f_o.normal=normalize(vec3(0.0,1.0,0.0));
	EmitVertex();

	gl_Position=MVPMatrix*vec4(vec3(position.xyz+(vec3(-0.5*size.x,size.y,0.5*size.z))),1.0);
	g2f_o.uv=vec2(0.0,1.0);
	g2f_o.normal=normalize(vec3(0.0,1.0,0.0));
	EmitVertex();
	
	gl_Position=MVPMatrix*vec4(vec3(position.xyz+(vec3(0.5*size.x,size.y,0.5*size.z))),1.0);
	g2f_o.uv=vec2(1.0,1.0);
	g2f_o.normal=normalize(vec3(0.0,1.0,0.0));
	EmitVertex();

	EndPrimitive();
	
	// -y
	gl_Position=MVPMatrix*vec4(vec3(position.xyz+(vec3(-0.5*size.x,0.0,-0.5*size.z))),1.0);
	g2f_o.uv=vec2(0.0,0.0);
	g2f_o.normal=normalize(vec3(0.0,-1.0,0.0));
	EmitVertex();

	gl_Position=MVPMatrix*vec4(vec3(position.xyz+(vec3(0.5*size.x,0.0,-0.5*size.z))),1.0);
	g2f_o.uv=vec2(1.0,0.0);
	g2f_o.normal=normalize(vec3(0.0,-1.0,0.0));
	EmitVertex();

	gl_Position=MVPMatrix*vec4(vec3(position.xyz+(vec3(-0.5*size.x,0.0,0.5*size.z))),1.0);
	g2f_o.uv=vec2(0.0,1.0);
	g2f_o.normal=normalize(vec3(0.0,-1.0,0.0));
	EmitVertex();
	
	gl_Position=MVPMatrix*vec4(vec3(position.xyz+(vec3(0.5*size.x,0.0,0.5*size.z))),1.0);
	g2f_o.uv=vec2(1.0,1.0);
	g2f_o.normal=normalize(vec3(0.0,-1.0,0.0));
	EmitVertex();

	EndPrimitive();

	// +z
	gl_Position=MVPMatrix*vec4(vec3(position.xyz+(vec3(-0.5*size.x,0.0,0.5*size.z))),1.0);
	g2f_o.uv=vec2(0.0,0.0);
	g2f_o.normal=normalize(vec3(0.0,0.0,1.0));
	EmitVertex();

	gl_Position=MVPMatrix*vec4(vec3(position.xyz+(vec3(0.5*size.x,0.0,0.5*size.z))),1.0);
	g2f_o.uv=vec2(1.0,0.0);
	g2f_o.normal=normalize(vec3(0.0,0.0,1.0));
	EmitVertex();

	gl_Position=MVPMatrix*vec4(vec3(position.xyz+(vec3(-0.5*size.x,size.y,0.5*size.z))),1.0);
	g2f_o.uv=vec2(0.0,1.0);
	g2f_o.normal=normalize(vec3(0.0,0.0,1.0));
	EmitVertex();
	
	gl_Position=MVPMatrix*vec4(vec3(position.xyz+(vec3(0.5*size.x,size.y,0.5*size.z))),1.0);
	g2f_o.uv=vec2(1.0,1.0);
	g2f_o.normal=normalize(vec3(0.0,0.0,1.0));
	EmitVertex();

	EndPrimitive();
	
	// -z
	gl_Position=MVPMatrix*vec4(vec3(position.xyz+(vec3(-0.5*size.x,0.0,-0.5*size.z))),1.0);
	g2f_o.uv=vec2(0.0,0.0);
	g2f_o.normal=normalize(vec3(0.0,0.0,-1.0));
	EmitVertex();

	gl_Position=MVPMatrix*vec4(vec3(position.xyz+(vec3(0.5*size.x,0.0,-0.5*size.z))),1.0);
	g2f_o.uv=vec2(1.0,0.0);
	g2f_o.normal=normalize(vec3(0.0,0.0,-1.0));
	EmitVertex();

	gl_Position=MVPMatrix*vec4(vec3(position.xyz+(vec3(-0.5*size.x,size.y,-0.5*size.z))),1.0);
	g2f_o.uv=vec2(0.0,1.0);
	g2f_o.normal=normalize(vec3(0.0,0.0,-1.0));
	EmitVertex();
	
	gl_Position=MVPMatrix*vec4(vec3(position.xyz+(vec3(0.5*size.x,size.y,-0.5*size.z))),1.0);
	g2f_o.uv=vec2(1.0,1.0);
	g2f_o.normal=normalize(vec3(0.0,0.0,-1.0));
	EmitVertex();

	EndPrimitive();
}

void Tri(vec4 position,vec2 size){
	// 底面
	gl_Position=MVPMatrix*vec4(vec3(position.xyz+(vec3(-0.5*size.x,0.0,-0.5*size.x))),1.0);
	g2f_o.uv=vec2(0.0,0.0);
	g2f_o.normal=normalize(vec3(0.0,-1.0,0.0));
	EmitVertex();

	gl_Position=MVPMatrix*vec4(vec3(position.xyz+(vec3(0.5*size.x,0.0,-0.5*size.x))),1.0);
	g2f_o.uv=vec2(1.0,0.0);
	g2f_o.normal=normalize(vec3(0.0,-1.0,0.0));
	EmitVertex();

	gl_Position=MVPMatrix*vec4(vec3(position.xyz+(vec3(-0.5*size.x,0.0,0.5*size.x))),1.0);
	g2f_o.uv=vec2(0.0,1.0);
	g2f_o.normal=normalize(vec3(0.0,-1.0,0.0));
	EmitVertex();
	
	gl_Position=MVPMatrix*vec4(vec3(position.xyz+(vec3(0.5*size.x,0.0,0.5*size.x))),1.0);
	g2f_o.uv=vec2(1.0,1.0);
	g2f_o.normal=normalize(vec3(0.0,-1.0,0.0));
	EmitVertex();

	EndPrimitive();

	// 三角形
	gl_Position=MVPMatrix*vec4(vec3(position.xyz+(vec3(-0.5*size.x,0.0,-0.5*size.x))),1.0);
	g2f_o.uv=vec2(0.0,0.0);
	g2f_o.normal=normalize(vec3(0.0,-1.0,0.0));
	EmitVertex();

	gl_Position=MVPMatrix*vec4(vec3(position.xyz+(vec3(0.5*size.x,0.0,-0.5*size.x))),1.0);
	g2f_o.uv=vec2(1.0,0.0);
	g2f_o.normal=normalize(vec3(0.0,-1.0,0.0));
	EmitVertex();

	gl_Position=MVPMatrix*vec4(vec3(position.xyz+(vec3(0.0,size.y,0.0))),1.0);
	g2f_o.uv=vec2(0.0,1.0);
	g2f_o.normal=normalize(vec3(0.0,-1.0,0.0));
	EmitVertex();

	EndPrimitive();

	// 三角形
	gl_Position=MVPMatrix*vec4(vec3(position.xyz+(vec3(-0.5*size.x,0.0,0.5*size.x))),1.0);
	g2f_o.uv=vec2(0.0,0.0);
	g2f_o.normal=normalize(vec3(0.0,-1.0,0.0));
	EmitVertex();

	gl_Position=MVPMatrix*vec4(vec3(position.xyz+(vec3(-0.5*size.x,0.0,-0.5*size.x))),1.0);
	g2f_o.uv=vec2(1.0,0.0);
	g2f_o.normal=normalize(vec3(0.0,-1.0,0.0));
	EmitVertex();

	gl_Position=MVPMatrix*vec4(vec3(position.xyz+(vec3(0.0,size.y,0.0))),1.0);
	g2f_o.uv=vec2(0.0,1.0);
	g2f_o.normal=normalize(vec3(0.0,-1.0,0.0));
	EmitVertex();

	EndPrimitive();

	// 三角形
	gl_Position=MVPMatrix*vec4(vec3(position.xyz+(vec3(0.5*size.x,0.0,0.5*size.x))),1.0);
	g2f_o.uv=vec2(0.0,0.0);
	g2f_o.normal=normalize(vec3(0.0,-1.0,0.0));
	EmitVertex();

	gl_Position=MVPMatrix*vec4(vec3(position.xyz+(vec3(-0.5*size.x,0.0,0.5*size.x))),1.0);
	g2f_o.uv=vec2(1.0,0.0);
	g2f_o.normal=normalize(vec3(0.0,-1.0,0.0));
	EmitVertex();

	gl_Position=MVPMatrix*vec4(vec3(position.xyz+(vec3(0.0,size.y,0.0))),1.0);
	g2f_o.uv=vec2(0.0,1.0);
	g2f_o.normal=normalize(vec3(0.0,-1.0,0.0));
	EmitVertex();

	EndPrimitive();

	// 三角形
	gl_Position=MVPMatrix*vec4(vec3(position.xyz+(vec3(0.5*size.x,0.0,-0.5*size.x))),1.0);
	g2f_o.uv=vec2(0.0,0.0);
	g2f_o.normal=normalize(vec3(0.0,-1.0,0.0));
	EmitVertex();

	gl_Position=MVPMatrix*vec4(vec3(position.xyz+(vec3(0.5*size.x,0.0,0.5*size.x))),1.0);
	g2f_o.uv=vec2(1.0,0.0);
	g2f_o.normal=normalize(vec3(0.0,-1.0,0.0));
	EmitVertex();

	gl_Position=MVPMatrix*vec4(vec3(position.xyz+(vec3(0.0,size.y,0.0))),1.0);
	g2f_o.uv=vec2(0.0,1.0);
	g2f_o.normal=normalize(vec3(0.0,-1.0,0.0));
	EmitVertex();

	EndPrimitive();
}

void Bill0(vec4 position,int id){
	
	vec3 size=vec3(1.0,2.0,1.0);
	for(int level=0;level<3;level++)
	{
		vec2 offset=vec2( rand(vec2(float(level+id),0.973))*2.0-1.0,rand(vec2(0.197,float(level+id)))*2.0-1.0 )*0.15;
		CreateCube(position+vec4(offset.x,0.0,offset.y,0.0),size);
		size*=vec3( rand(vec2(float((level+id)*10.0),0.156)),rand(vec2(0.732,float((level+id)*10.0))),rand(vec2(0.9755,float((level+id)*10.0)))+vec3(0.25) );
	}
	CreateCube(position+vec4(0.0,0.0,0.0,0.0),vec3(1.0,0.05,1.0));

}

void Bill1(vec4 position,int id){	
	// 後で乱数にする
	float height=1.0;
	float halfHeight=height;
	float width=0.5;

	// キューブ
	for(int i=0;i<3;i++){
		CreateCube(position,vec3(width,height,width));
		halfHeight*=0.5;
		height+=halfHeight;
		width*=0.8;
	}

	// 三角柱
	Tri(position+vec4(0.0,height-halfHeight,0.0,0.0) ,vec2(width));
}

void Bill2(vec4 position,int id){

}

void main()
{
	//Bill0(gl_in[0].gl_Position+vec4(v2g_o[0].randPos,0.0),v2g_o[0].id);
	Bill1(gl_in[0].gl_Position+vec4(v2g_o[0].randPos,0.0),v2g_o[0].id);
}

)"