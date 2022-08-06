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
	vec3 randPos;
	int id;
};

struct g2f{
	vec2 uv;
	vec3 normal;
	float id;
	float billID;
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
	g2f_o.id=float(v2g_o[0].id); g2f_o.billID=1.0; EmitVertex(); 
	
	gl_Position=MVPMatrix*vec4(vec3(position.xyz+(vec3(0.5*size.x,0.0,0.5*size.z))),1.0);
	g2f_o.uv=vec2(1.0,0.0);
	g2f_o.normal=normalize(vec3(1.0,0.0,0.0));
	g2f_o.id=float(v2g_o[0].id); g2f_o.billID=1.0; EmitVertex(); 

	gl_Position=MVPMatrix*vec4(vec3(position.xyz+(vec3(0.5*size.x,size.y,-0.5*size.z))),1.0);
	g2f_o.uv=vec2(0.0,1.0);
	g2f_o.normal=normalize(vec3(1.0,0.0,0.0));
	g2f_o.id=float(v2g_o[0].id); g2f_o.billID=1.0; EmitVertex(); 
	
	gl_Position=MVPMatrix*vec4(vec3(position.xyz+(vec3(0.5*size.x,size.y,0.5*size.z))),1.0);
	g2f_o.uv=vec2(1.0,1.0);
	g2f_o.normal=normalize(vec3(1.0,0.0,0.0));
	g2f_o.id=float(v2g_o[0].id); g2f_o.billID=1.0; EmitVertex(); 

	EndPrimitive();
	
	// -x
	gl_Position=MVPMatrix*vec4(vec3(position.xyz+(vec3(-0.5*size.x,0.0,-0.5*size.z))),1.0);
	g2f_o.uv=vec2(0.0,0.0);
	g2f_o.normal=normalize(vec3(-1.0,0.0,0.0));
	g2f_o.id=float(v2g_o[0].id); g2f_o.billID=1.0; EmitVertex(); 

	gl_Position=MVPMatrix*vec4(vec3(position.xyz+(vec3(-0.5*size.x,0.0,0.5*size.z))),1.0);
	g2f_o.uv=vec2(1.0,0.0);
	g2f_o.normal=normalize(vec3(-1.0,0.0,0.0));
	g2f_o.id=float(v2g_o[0].id); g2f_o.billID=1.0; EmitVertex(); 

	gl_Position=MVPMatrix*vec4(vec3(position.xyz+(vec3(-0.5*size.x,size.y,-0.5*size.z))),1.0);
	g2f_o.uv=vec2(0.0,1.0);
	g2f_o.normal=normalize(vec3(-1.0,0.0,0.0));
	g2f_o.id=float(v2g_o[0].id); g2f_o.billID=1.0; EmitVertex(); 
	
	gl_Position=MVPMatrix*vec4(vec3(position.xyz+(vec3(-0.5*size.x,size.y,0.5*size.z))),1.0);
	g2f_o.uv=vec2(1.0,1.0);
	g2f_o.normal=normalize(vec3(-1.0,0.0,0.0));
	g2f_o.id=float(v2g_o[0].id); g2f_o.billID=1.0; EmitVertex(); 

	EndPrimitive();

	// +y
	gl_Position=MVPMatrix*vec4(vec3(position.xyz+(vec3(-0.5*size.x,size.y,-0.5*size.z))),1.0);
	g2f_o.uv=vec2(0.0,0.0);
	g2f_o.normal=normalize(vec3(0.0,1.0,0.0));
	g2f_o.id=float(v2g_o[0].id); g2f_o.billID=1.0; EmitVertex(); 

	gl_Position=MVPMatrix*vec4(vec3(position.xyz+(vec3(0.5*size.x,size.y,-0.5*size.z))),1.0);
	g2f_o.uv=vec2(1.0,0.0);
	g2f_o.normal=normalize(vec3(0.0,1.0,0.0));
	g2f_o.id=float(v2g_o[0].id); g2f_o.billID=1.0; EmitVertex(); 

	gl_Position=MVPMatrix*vec4(vec3(position.xyz+(vec3(-0.5*size.x,size.y,0.5*size.z))),1.0);
	g2f_o.uv=vec2(0.0,1.0);
	g2f_o.normal=normalize(vec3(0.0,1.0,0.0));
	g2f_o.id=float(v2g_o[0].id); g2f_o.billID=1.0; EmitVertex(); 
	
	gl_Position=MVPMatrix*vec4(vec3(position.xyz+(vec3(0.5*size.x,size.y,0.5*size.z))),1.0);
	g2f_o.uv=vec2(1.0,1.0);
	g2f_o.normal=normalize(vec3(0.0,1.0,0.0));
	g2f_o.id=float(v2g_o[0].id); g2f_o.billID=1.0; EmitVertex(); 

	EndPrimitive();
	
	// -y
	gl_Position=MVPMatrix*vec4(vec3(position.xyz+(vec3(-0.5*size.x,0.0,-0.5*size.z))),1.0);
	g2f_o.uv=vec2(0.0,0.0);
	g2f_o.normal=normalize(vec3(0.0,-1.0,0.0));
	g2f_o.id=float(v2g_o[0].id); g2f_o.billID=1.0; EmitVertex(); 

	gl_Position=MVPMatrix*vec4(vec3(position.xyz+(vec3(0.5*size.x,0.0,-0.5*size.z))),1.0);
	g2f_o.uv=vec2(1.0,0.0);
	g2f_o.normal=normalize(vec3(0.0,-1.0,0.0));
	g2f_o.id=float(v2g_o[0].id); g2f_o.billID=1.0; EmitVertex(); 

	gl_Position=MVPMatrix*vec4(vec3(position.xyz+(vec3(-0.5*size.x,0.0,0.5*size.z))),1.0);
	g2f_o.uv=vec2(0.0,1.0);
	g2f_o.normal=normalize(vec3(0.0,-1.0,0.0));
	g2f_o.id=float(v2g_o[0].id); g2f_o.billID=1.0; EmitVertex(); 
	
	gl_Position=MVPMatrix*vec4(vec3(position.xyz+(vec3(0.5*size.x,0.0,0.5*size.z))),1.0);
	g2f_o.uv=vec2(1.0,1.0);
	g2f_o.normal=normalize(vec3(0.0,-1.0,0.0));
	g2f_o.id=float(v2g_o[0].id); g2f_o.billID=1.0; EmitVertex(); 

	EndPrimitive();

	// +z
	gl_Position=MVPMatrix*vec4(vec3(position.xyz+(vec3(-0.5*size.x,0.0,0.5*size.z))),1.0);
	g2f_o.uv=vec2(0.0,0.0);
	g2f_o.normal=normalize(vec3(0.0,0.0,1.0));
	g2f_o.id=float(v2g_o[0].id); g2f_o.billID=1.0; EmitVertex(); 

	gl_Position=MVPMatrix*vec4(vec3(position.xyz+(vec3(0.5*size.x,0.0,0.5*size.z))),1.0);
	g2f_o.uv=vec2(1.0,0.0);
	g2f_o.normal=normalize(vec3(0.0,0.0,1.0));
	g2f_o.id=float(v2g_o[0].id); g2f_o.billID=1.0; EmitVertex(); 

	gl_Position=MVPMatrix*vec4(vec3(position.xyz+(vec3(-0.5*size.x,size.y,0.5*size.z))),1.0);
	g2f_o.uv=vec2(0.0,1.0);
	g2f_o.normal=normalize(vec3(0.0,0.0,1.0));
	g2f_o.id=float(v2g_o[0].id); g2f_o.billID=1.0; EmitVertex(); 
	
	gl_Position=MVPMatrix*vec4(vec3(position.xyz+(vec3(0.5*size.x,size.y,0.5*size.z))),1.0);
	g2f_o.uv=vec2(1.0,1.0);
	g2f_o.normal=normalize(vec3(0.0,0.0,1.0));
	g2f_o.id=float(v2g_o[0].id); g2f_o.billID=1.0; EmitVertex(); 

	EndPrimitive();
	
	// -z
	gl_Position=MVPMatrix*vec4(vec3(position.xyz+(vec3(-0.5*size.x,0.0,-0.5*size.z))),1.0);
	g2f_o.uv=vec2(0.0,0.0);
	g2f_o.normal=normalize(vec3(0.0,0.0,-1.0));
	g2f_o.id=float(v2g_o[0].id); g2f_o.billID=1.0; EmitVertex(); 

	gl_Position=MVPMatrix*vec4(vec3(position.xyz+(vec3(0.5*size.x,0.0,-0.5*size.z))),1.0);
	g2f_o.uv=vec2(1.0,0.0);
	g2f_o.normal=normalize(vec3(0.0,0.0,-1.0));
	g2f_o.id=float(v2g_o[0].id); g2f_o.billID=1.0; EmitVertex(); 

	gl_Position=MVPMatrix*vec4(vec3(position.xyz+(vec3(-0.5*size.x,size.y,-0.5*size.z))),1.0);
	g2f_o.uv=vec2(0.0,1.0);
	g2f_o.normal=normalize(vec3(0.0,0.0,-1.0));
	g2f_o.id=float(v2g_o[0].id); g2f_o.billID=1.0; EmitVertex(); 
	
	gl_Position=MVPMatrix*vec4(vec3(position.xyz+(vec3(0.5*size.x,size.y,-0.5*size.z))),1.0);
	g2f_o.uv=vec2(1.0,1.0);
	g2f_o.normal=normalize(vec3(0.0,0.0,-1.0));
	g2f_o.id=float(v2g_o[0].id); g2f_o.billID=1.0; EmitVertex(); 

	EndPrimitive();
}

void Tri(vec4 position,vec2 size){
	// ’ê–Ê
	gl_Position=MVPMatrix*vec4(vec3(position.xyz+(vec3(-0.5*size.x,0.0,-0.5*size.x))),1.0);
	g2f_o.uv=vec2(0.0,0.0);
	g2f_o.normal=normalize(vec3(0.0,-1.0,0.0));
	g2f_o.id=float(v2g_o[0].id); g2f_o.billID=1.0; EmitVertex(); 

	gl_Position=MVPMatrix*vec4(vec3(position.xyz+(vec3(0.5*size.x,0.0,-0.5*size.x))),1.0);
	g2f_o.uv=vec2(1.0,0.0);
	g2f_o.normal=normalize(vec3(0.0,-1.0,0.0));
	g2f_o.id=float(v2g_o[0].id); g2f_o.billID=1.0; EmitVertex(); 

	gl_Position=MVPMatrix*vec4(vec3(position.xyz+(vec3(-0.5*size.x,0.0,0.5*size.x))),1.0);
	g2f_o.uv=vec2(0.0,1.0);
	g2f_o.normal=normalize(vec3(0.0,-1.0,0.0));
	g2f_o.id=float(v2g_o[0].id); g2f_o.billID=1.0; EmitVertex(); 
	
	gl_Position=MVPMatrix*vec4(vec3(position.xyz+(vec3(0.5*size.x,0.0,0.5*size.x))),1.0);
	g2f_o.uv=vec2(1.0,1.0);
	g2f_o.normal=normalize(vec3(0.0,-1.0,0.0));
	g2f_o.id=float(v2g_o[0].id); g2f_o.billID=1.0; EmitVertex(); 

	EndPrimitive();

	// ŽOŠpŒ`
	gl_Position=MVPMatrix*vec4(vec3(position.xyz+(vec3(-0.5*size.x,0.0,-0.5*size.x))),1.0);
	g2f_o.uv=vec2(0.0,0.0);
	g2f_o.normal=normalize(vec3(0.0,-1.0,0.0));
	g2f_o.id=float(v2g_o[0].id); g2f_o.billID=1.0; EmitVertex(); 

	gl_Position=MVPMatrix*vec4(vec3(position.xyz+(vec3(0.5*size.x,0.0,-0.5*size.x))),1.0);
	g2f_o.uv=vec2(1.0,0.0);
	g2f_o.normal=normalize(vec3(0.0,-1.0,0.0));
	g2f_o.id=float(v2g_o[0].id); g2f_o.billID=1.0; EmitVertex(); 

	gl_Position=MVPMatrix*vec4(vec3(position.xyz+(vec3(0.0,size.y,0.0))),1.0);
	g2f_o.uv=vec2(0.0,1.0);
	g2f_o.normal=normalize(vec3(0.0,-1.0,0.0));
	g2f_o.id=float(v2g_o[0].id); g2f_o.billID=1.0; EmitVertex(); 

	EndPrimitive();

	// ŽOŠpŒ`
	gl_Position=MVPMatrix*vec4(vec3(position.xyz+(vec3(-0.5*size.x,0.0,0.5*size.x))),1.0);
	g2f_o.uv=vec2(0.0,0.0);
	g2f_o.normal=normalize(vec3(0.0,-1.0,0.0));
	g2f_o.id=float(v2g_o[0].id); g2f_o.billID=1.0; EmitVertex(); 

	gl_Position=MVPMatrix*vec4(vec3(position.xyz+(vec3(-0.5*size.x,0.0,-0.5*size.x))),1.0);
	g2f_o.uv=vec2(1.0,0.0);
	g2f_o.normal=normalize(vec3(0.0,-1.0,0.0));
	g2f_o.id=float(v2g_o[0].id); g2f_o.billID=1.0; EmitVertex(); 

	gl_Position=MVPMatrix*vec4(vec3(position.xyz+(vec3(0.0,size.y,0.0))),1.0);
	g2f_o.uv=vec2(0.0,1.0);
	g2f_o.normal=normalize(vec3(0.0,-1.0,0.0));
	g2f_o.id=float(v2g_o[0].id); g2f_o.billID=1.0; EmitVertex(); 

	EndPrimitive();

	// ŽOŠpŒ`
	gl_Position=MVPMatrix*vec4(vec3(position.xyz+(vec3(0.5*size.x,0.0,0.5*size.x))),1.0);
	g2f_o.uv=vec2(0.0,0.0);
	g2f_o.normal=normalize(vec3(0.0,-1.0,0.0));
	g2f_o.id=float(v2g_o[0].id); g2f_o.billID=1.0; EmitVertex(); 

	gl_Position=MVPMatrix*vec4(vec3(position.xyz+(vec3(-0.5*size.x,0.0,0.5*size.x))),1.0);
	g2f_o.uv=vec2(1.0,0.0);
	g2f_o.normal=normalize(vec3(0.0,-1.0,0.0));
	g2f_o.id=float(v2g_o[0].id); g2f_o.billID=1.0; EmitVertex(); 

	gl_Position=MVPMatrix*vec4(vec3(position.xyz+(vec3(0.0,size.y,0.0))),1.0);
	g2f_o.uv=vec2(0.0,1.0);
	g2f_o.normal=normalize(vec3(0.0,-1.0,0.0));
	g2f_o.id=float(v2g_o[0].id); g2f_o.billID=1.0; EmitVertex(); 

	EndPrimitive();

	// ŽOŠpŒ`
	gl_Position=MVPMatrix*vec4(vec3(position.xyz+(vec3(0.5*size.x,0.0,-0.5*size.x))),1.0);
	g2f_o.uv=vec2(0.0,0.0);
	g2f_o.normal=normalize(vec3(0.0,-1.0,0.0));
	g2f_o.id=float(v2g_o[0].id); g2f_o.billID=1.0; EmitVertex(); 

	gl_Position=MVPMatrix*vec4(vec3(position.xyz+(vec3(0.5*size.x,0.0,0.5*size.x))),1.0);
	g2f_o.uv=vec2(1.0,0.0);
	g2f_o.normal=normalize(vec3(0.0,-1.0,0.0));
	g2f_o.id=float(v2g_o[0].id); g2f_o.billID=1.0; EmitVertex(); 

	gl_Position=MVPMatrix*vec4(vec3(position.xyz+(vec3(0.0,size.y,0.0))),1.0);
	g2f_o.uv=vec2(0.0,1.0);
	g2f_o.normal=normalize(vec3(0.0,-1.0,0.0));
	g2f_o.id=float(v2g_o[0].id); g2f_o.billID=1.0; EmitVertex(); 

	EndPrimitive();
}

void Bill0(vec4 position,int id){
	
	vec3 size=vec3(0.5,2.0,0.5);
	vec2 offset=vec2(0.0);
	float so=0.8;

	for(int level=0;level<3;level++)
	{
		CreateCube(position+vec4(offset.x,0.0,offset.y,0.0),size);

		offset=vec2( (rand(vec2(float(level+id),0.973))*2.0-1.0)*size.x*0.5 , (rand(vec2(0.197,float(level+id)))*2.0-1.0)*size.z*0.5 );
		if(level==0)
		{
			size=vec3( 0.5+rand(vec2(float((level+id)*10.0),0.156))*so , size.y*0.5 , rand(vec2(0.9755,float((level+id)*10.0)))*so );
		}
		else if(level==1)
		{
			size=vec3( rand(vec2(0.156,float((level+id)*10.0)))*so , size.y*0.5 , 0.5+rand(vec2(float((level+id)*10.0),0.9755))*so );
		}
	}
	CreateCube(position+vec4(0.0,0.0,0.0,0.0),vec3(1.0,0.05,1.0));

}

void Bill1(vec4 position,int id){	
	// Œã‚Å—”‚É‚·‚é
	float height=1.0;
	float halfHeight=height;
	float width=0.5;

	// ƒLƒ…[ƒu
	for(int i=0;i<3;i++){
		CreateCube(position,vec3(width,height,width));
		halfHeight*=0.5;
		height+=halfHeight;
		width*=0.8;
	}

	// ŽOŠp’Œ
	Tri(position+vec4(0.0,height-halfHeight,0.0,0.0) ,vec2(width));
}

void Bill2(vec4 position,int id){
	float height=2.0;
	float radius=0.25,num=12.0,pi=3.1415,offnum=2.0;
	float omega=(2.0*pi)/num;
	float angle=0.0,nextAngle=omega;

	for(float p=0.0;p<num;p++){
		float off=0.0,rd=rand(vec2(p,position.x+position.z));
		if(offnum>0.0&& (rd>=0.5 && rd<0.7) ){
			off=rand(vec2(position.x+position.z,p))*pi;
			offnum--;
		}
		angle=nextAngle,nextAngle+=omega+off;
		// float angle=omega*p,nextAngle=omega*(p+1.0)+off;

		gl_Position=MVPMatrix*vec4(vec3(position.xyz+(vec3(radius*cos(angle),0.0,radius*sin(angle)))),1.0);
		g2f_o.uv=vec2(0.0,0.0);
		g2f_o.normal=normalize(vec3(radius*cos(angle),0.0,radius*sin(angle)));
		g2f_o.id=float(v2g_o[0].id); g2f_o.billID=2.0; EmitVertex(); 

		gl_Position=MVPMatrix*vec4(vec3(position.xyz+(vec3(radius*cos(angle),height,radius*sin(angle)))),1.0);
		g2f_o.uv=vec2(1.0,0.0);
		g2f_o.normal=normalize(vec3(radius*cos(angle),0.0,radius*sin(angle)));
		g2f_o.id=float(v2g_o[0].id); g2f_o.billID=2.0; EmitVertex(); 

		gl_Position=MVPMatrix*vec4(vec3(position.xyz+(vec3(radius*cos(nextAngle),0.0,radius*sin(nextAngle)))),1.0);
		g2f_o.uv=vec2(0.0,1.0);
		g2f_o.normal=normalize(vec3(radius*cos(nextAngle),0.0,radius*sin(nextAngle)));
		g2f_o.id=float(v2g_o[0].id); g2f_o.billID=2.0; EmitVertex(); 
	
		gl_Position=MVPMatrix*vec4(vec3(position.xyz+(vec3(radius*cos(nextAngle),height,radius*sin(nextAngle)))),1.0);
		g2f_o.uv=vec2(1.0,1.0);
		g2f_o.normal=normalize(vec3(radius*cos(nextAngle),0.0,radius*sin(nextAngle)));
		g2f_o.id=float(v2g_o[0].id); g2f_o.billID=2.0; EmitVertex(); 

		EndPrimitive();

		// ãŠW
		gl_Position=MVPMatrix*vec4(vec3(position.xyz+(vec3(radius*cos(angle),height,radius*sin(angle)))),1.0);
		g2f_o.uv=vec2(1.0,0.0);
		g2f_o.normal=normalize(vec3(0.0,-1.0,0.0));
		g2f_o.id=float(v2g_o[0].id); g2f_o.billID=2.0; EmitVertex(); 

		gl_Position=MVPMatrix*vec4(vec3(position.xyz+(vec3(radius*cos(nextAngle),height,radius*sin(nextAngle)))),1.0);
		g2f_o.uv=vec2(0.0,1.0);
		g2f_o.normal=normalize(vec3(0.0,-1.0,0.0));
		g2f_o.id=float(v2g_o[0].id); g2f_o.billID=2.0; EmitVertex(); 
	
		gl_Position=MVPMatrix*vec4(vec3(position.xyz+ vec3(0.0,height,0.0) ),1.0);
		g2f_o.uv=vec2(1.0,1.0);
		g2f_o.normal=normalize(vec3(0.0,-1.0,0.0));
		g2f_o.id=float(v2g_o[0].id); g2f_o.billID=2.0; EmitVertex(); 

		EndPrimitive();

		if(angle>=2.0*pi)break;
	}
}

void main()
{
	int billID=int(floor( rand(vec2(v2g_o[0].id*100,v2g_o[0].id*100.0))*3.0 ));
	//int billID=0;

	if(billID==0)
	{
		Bill0(gl_in[0].gl_Position+vec4(v2g_o[0].randPos,0.0),v2g_o[0].id);
	}
	else if(billID==1)
	{
		Bill1(gl_in[0].gl_Position+vec4(v2g_o[0].randPos,0.0),v2g_o[0].id);
	}
	else if(billID==2)
	{
		//Bill0(gl_in[0].gl_Position+vec4(v2g_o[0].randPos,0.0),v2g_o[0].id);	
		Bill2(gl_in[0].gl_Position+vec4(v2g_o[0].randPos,0.0),v2g_o[0].id);	
	}
}

)"