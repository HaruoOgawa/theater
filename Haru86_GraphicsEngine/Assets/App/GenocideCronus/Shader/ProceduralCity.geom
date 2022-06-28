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
};

struct g2f{
	vec2 uv;
};

in v2g v2g_o[]; 

out g2f g2f_o; 

void SampleBill(vec4 position){
	gl_Position=MVPMatrix*vec4(vec3(position.xyz+normalize(vec3(-0.5,-0.5,0.0))),1.0);
	g2f_o.uv=vec2(0.0,0.0);
	EmitVertex();

	gl_Position=MVPMatrix*vec4(vec3(position.xyz+normalize(vec3(-0.5,0.5,0.0))),1.0);
	g2f_o.uv=vec2(0.0,1.0);
	EmitVertex();

	gl_Position=MVPMatrix*vec4(vec3(position.xyz+normalize(vec3(0.5,0.5,0.0))),1.0);
	g2f_o.uv=vec2(1.0,1.0);
	EmitVertex();
	
	EndPrimitive();

	gl_Position=MVPMatrix*vec4(vec3(position.xyz+normalize(vec3(0.5,0.5,0.0))),1.0);
	g2f_o.uv=vec2(1.0,1.0);
	EmitVertex();

	gl_Position=MVPMatrix*vec4(vec3(position.xyz+normalize(vec3(0.5,-0.5,0.0))),1.0);
	g2f_o.uv=vec2(1.0,0.0);
	EmitVertex();

	gl_Position=MVPMatrix*vec4(vec3(position.xyz+normalize(vec3(-0.5,-0.5,0.0))),1.0);
	g2f_o.uv=vec2(0.0,0.0);
	EmitVertex();
	
	EndPrimitive();
}

void CreateCube(vec4 position,vec3 size){
	// +y
	gl_Position=MVPMatrix*vec4(vec3(position.xyz+normalize(vec3(-0.5*size.x,0.5*size.y,-0.5*size.z))),1.0);
	g2f_o.uv=vec2(0.0,0.0);
	EmitVertex();

	gl_Position=MVPMatrix*vec4(vec3(position.xyz+normalize(vec3(0.5*size.x,0.5*size.y,-0.5*size.z))),1.0);
	g2f_o.uv=vec2(1.0,0.0);
	EmitVertex();

	gl_Position=MVPMatrix*vec4(vec3(position.xyz+normalize(vec3(-0.5*size.x,0.5*size.y,0.5*size.z))),1.0);
	g2f_o.uv=vec2(0.0,1.0);
	EmitVertex();
	
	gl_Position=MVPMatrix*vec4(vec3(position.xyz+normalize(vec3(0.5*size.x,0.5*size.y,0.5*size.z))),1.0);
	g2f_o.uv=vec2(1.0,1.0);
	EmitVertex();

	EndPrimitive();
	
	// -y
	gl_Position=MVPMatrix*vec4(vec3(position.xyz+normalize(vec3(-0.5*size.x,-0.5*size.y,-0.5*size.z))),1.0);
	g2f_o.uv=vec2(0.0,0.0);
	EmitVertex();

	gl_Position=MVPMatrix*vec4(vec3(position.xyz+normalize(vec3(0.5*size.x,-0.5*size.y,-0.5*size.z))),1.0);
	g2f_o.uv=vec2(1.0,0.0);
	EmitVertex();

	gl_Position=MVPMatrix*vec4(vec3(position.xyz+normalize(vec3(-0.5*size.x,-0.5*size.y,0.5*size.z))),1.0);
	g2f_o.uv=vec2(0.0,1.0);
	EmitVertex();
	
	gl_Position=MVPMatrix*vec4(vec3(position.xyz+normalize(vec3(0.5*size.x,-0.5*size.y,0.5*size.z))),1.0);
	g2f_o.uv=vec2(1.0,1.0);
	EmitVertex();

	EndPrimitive();

	// +z
	gl_Position=MVPMatrix*vec4(vec3(position.xyz+normalize(vec3(-0.5*size.x,-0.5*size.y,0.5*size.z))),1.0);
	g2f_o.uv=vec2(0.0,0.0);
	EmitVertex();

	gl_Position=MVPMatrix*vec4(vec3(position.xyz+normalize(vec3(0.5*size.x,-0.5*size.y,0.5*size.z))),1.0);
	g2f_o.uv=vec2(1.0,0.0);
	EmitVertex();

	gl_Position=MVPMatrix*vec4(vec3(position.xyz+normalize(vec3(-0.5*size.x,0.5*size.y,0.5*size.z))),1.0);
	g2f_o.uv=vec2(0.0,1.0);
	EmitVertex();
	
	gl_Position=MVPMatrix*vec4(vec3(position.xyz+normalize(vec3(0.5*size.x,0.5*size.y,0.5*size.z))),1.0);
	g2f_o.uv=vec2(1.0,1.0);
	EmitVertex();

	EndPrimitive();
	
	// -z
	gl_Position=MVPMatrix*vec4(vec3(position.xyz+normalize(vec3(-0.5*size.x,-0.5*size.y,-0.5*size.z))),1.0);
	g2f_o.uv=vec2(0.0,0.0);
	EmitVertex();

	gl_Position=MVPMatrix*vec4(vec3(position.xyz+normalize(vec3(0.5*size.x,-0.5*size.y,-0.5*size.z))),1.0);
	g2f_o.uv=vec2(1.0,0.0);
	EmitVertex();

	gl_Position=MVPMatrix*vec4(vec3(position.xyz+normalize(vec3(-0.5*size.x,0.5*size.y,-0.5*size.z))),1.0);
	g2f_o.uv=vec2(0.0,1.0);
	EmitVertex();
	
	gl_Position=MVPMatrix*vec4(vec3(position.xyz+normalize(vec3(0.5*size.x,0.5*size.y,-0.5*size.z))),1.0);
	g2f_o.uv=vec2(1.0,1.0);
	EmitVertex();

	EndPrimitive();
	
	// +x
	gl_Position=MVPMatrix*vec4(vec3(position.xyz+normalize(vec3(0.5*size.x,-0.5*size.y,-0.5*size.z))),1.0);
	g2f_o.uv=vec2(0.0,0.0);
	EmitVertex();

	gl_Position=MVPMatrix*vec4(vec3(position.xyz+normalize(vec3(0.5*size.x,-0.5*size.y,0.5*size.z))),1.0);
	g2f_o.uv=vec2(1.0,0.0);
	EmitVertex();

	gl_Position=MVPMatrix*vec4(vec3(position.xyz+normalize(vec3(0.5*size.x,0.5*size.y,-0.5*size.z))),1.0);
	g2f_o.uv=vec2(0.0,1.0);
	EmitVertex();
	
	gl_Position=MVPMatrix*vec4(vec3(position.xyz+normalize(vec3(0.5*size.x,0.5*size.y,0.5*size.z))),1.0);
	g2f_o.uv=vec2(1.0,1.0);
	EmitVertex();

	EndPrimitive();
	
	// -x
	gl_Position=MVPMatrix*vec4(vec3(position.xyz+normalize(vec3(-0.5*size.x,-0.5*size.y,-0.5*size.z))),1.0);
	g2f_o.uv=vec2(0.0,0.0);
	EmitVertex();

	gl_Position=MVPMatrix*vec4(vec3(position.xyz+normalize(vec3(-0.5*size.x,-0.5*size.y,0.5*size.z))),1.0);
	g2f_o.uv=vec2(1.0,0.0);
	EmitVertex();

	gl_Position=MVPMatrix*vec4(vec3(position.xyz+normalize(vec3(-0.5*size.x,0.5*size.y,-0.5*size.z))),1.0);
	g2f_o.uv=vec2(0.0,1.0);
	EmitVertex();
	
	gl_Position=MVPMatrix*vec4(vec3(position.xyz+normalize(vec3(-0.5*size.x,0.5*size.y,0.5*size.z))),1.0);
	g2f_o.uv=vec2(1.0,1.0);
	EmitVertex();

	EndPrimitive();
}

float a[3][4] = {
	{1.0,1.0,1.0,1.0},
	{-1.0,-1.0,1.0,1.0},
	{-1.0,1.0,-1.0,1.0}
};

void Cube(vec4 position,vec3 size){
	for(int axis=0;axis<3;axis++)
	{
		for(int axisSign=0;axisSign<2;axisSign++)
		{
			//
			float ppppSing=(axisSign==0)? 1.0 : -1.0;

			// 
			gl_Position=MVPMatrix*vec4(vec3(position.xyz+normalize(vec3(0.5*size.x,-0.5*size.y,-0.5*size.z))),1.0);
			g2f_o.uv=vec2(0.0,0.0);
			EmitVertex();

			gl_Position=MVPMatrix*vec4(vec3(position.xyz+normalize(vec3(0.5*size.x,-0.5*size.y,0.5*size.z))),1.0);
			g2f_o.uv=vec2(1.0,0.0);
			EmitVertex();

			gl_Position=MVPMatrix*vec4(vec3(position.xyz+normalize(vec3(0.5*size.x,0.5*size.y,-0.5*size.z))),1.0);
			g2f_o.uv=vec2(0.0,1.0);
			EmitVertex();
	
			gl_Position=MVPMatrix*vec4(vec3(position.xyz+normalize(vec3(0.5*size.x,0.5*size.y,0.5*size.z))),1.0);
			g2f_o.uv=vec2(1.0,1.0);
			EmitVertex();

			EndPrimitive();
		}
	}
}

void Bill0(vec4 position){
	CreateCube(position+vec4(0.0,0.0,0.0,0.0),vec3(1.0,0.05,1.0));
	CreateCube(position+vec4(0.5,0.5,0.0,0.0),vec3(0.01,0.025,0.005));
	//CreateCube(position+vec4(0.0,0.3,0.0,0.0),vec3(0.7,0.1,0.3));
	//CreateCube(position+vec4(0.0,0.6,0.0,0.0),vec3(0.75,0.1,0.3));
	//CreateCube(position+vec4(0.0,0.0,0.0,0.0),vec3(0.6,2.0,0.25));
}

void main()
{
	Bill0(gl_in[0].gl_Position+vec4(v2g_o[0].randPos,0.0));
	
}

)"