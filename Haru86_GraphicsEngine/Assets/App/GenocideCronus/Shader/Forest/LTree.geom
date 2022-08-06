R"(

#version 410

#define PI 3.14159265

layout(lines) in;
layout(triangle_strip,max_vertices =144) out;
//layout(line_strip,256) out;

in vec3 LocalNormal[];
in float v2g_treeRadius[];

uniform mat4 MVPMatrix;
uniform mat4 MMatrix;
uniform mat4 VMatrix;
uniform mat4 PMatrix;
uniform float _time;
uniform float _deltaTime;
uniform vec3 _CameraPos;

// Tube Uniform
uniform float _TreeMaxRadius;
uniform int _TreeSegment;

out vec3 WorldNormal;

void main()
{
	vec4 pos0 = gl_in[0].gl_Position;
	vec4 pos1 = gl_in[1].gl_Position;

	vec3 tangent = normalize(pos1.xyz-pos0.xyz);
	vec3 normal	 = normalize(LocalNormal[0]);
	vec3 bionormal = normalize(cross(tangent,normal));

	float angle = 2.0 * PI / float(_TreeSegment);

	// ‚±‚±‚ÌŒvŽZ‚Í‚½‚Ô‚ñŒã‚ÅŒ©’¼‚·•K—v‚ª‚ ‚é >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
	for(int n=0; n<(_TreeSegment+1); n++)
	{
		float theta0 =  angle * (float(n));
		vec3 offVec0 = v2g_treeRadius[0] * normalize(cos(theta0)*bionormal + sin(theta0)*normal);

		float theta1 =  angle * (1.0 + float(n));
		vec3 offVec1 = v2g_treeRadius[0] * normalize(cos(theta1)*bionormal + sin(theta1)*normal);

		// triangle0
		gl_Position = MVPMatrix * vec4(pos0.xyz+offVec0,1.0);
		WorldNormal = (MMatrix * vec4(normalize(offVec0),0.0)).xyz;
		EmitVertex();

		gl_Position = MVPMatrix * vec4(pos0.xyz+offVec1,1.0);
		WorldNormal = (MMatrix * vec4(normalize(offVec1),0.0)).xyz;
		EmitVertex();

		gl_Position = MVPMatrix * vec4(pos1.xyz+offVec1,1.0);
		WorldNormal = (MMatrix * vec4(normalize(offVec1),0.0)).xyz;
		EmitVertex();

		EndPrimitive();

		// triangle1
		gl_Position = MVPMatrix * vec4(pos0.xyz+offVec0,1.0);
		WorldNormal = (MMatrix * vec4(normalize(offVec0),0.0)).xyz;
		EmitVertex();

		gl_Position = MVPMatrix * vec4(pos1.xyz+offVec1,1.0);
		WorldNormal = (MMatrix * vec4(normalize(offVec1),0.0)).xyz;
		EmitVertex();

		gl_Position = MVPMatrix * vec4(pos1.xyz+offVec0,1.0);
		WorldNormal = (MMatrix * vec4(normalize(offVec0),0.0)).xyz;
		EmitVertex();

		EndPrimitive();

		// ã‚ÌŠW
		gl_Position = MVPMatrix * vec4(pos1.xyz,1.0);
		WorldNormal = (MMatrix * vec4(normalize(tangent),0.0)).xyz;
		EmitVertex();

		gl_Position = MVPMatrix * vec4(pos1.xyz+offVec1,1.0);
		WorldNormal = (MMatrix * vec4(normalize(tangent),0.0)).xyz;
		EmitVertex();

		gl_Position = MVPMatrix * vec4(pos1.xyz+offVec0,1.0);
		WorldNormal = (MMatrix * vec4(normalize(tangent),0.0)).xyz;
		EmitVertex();

		EndPrimitive();

		// ‰º‚ÌŠW
		gl_Position = MVPMatrix * vec4(pos0.xyz,1.0);
		WorldNormal = (MMatrix * vec4(normalize(tangent),0.0)).xyz;
		EmitVertex();

		gl_Position = MVPMatrix * vec4(pos0.xyz+offVec1,1.0);
		WorldNormal = (MMatrix * vec4(normalize(tangent),0.0)).xyz;
		EmitVertex();

		gl_Position = MVPMatrix * vec4(pos0.xyz+offVec0,1.0);
		WorldNormal = (MMatrix * vec4(normalize(tangent),0.0)).xyz;
		EmitVertex();

		EndPrimitive();
	}

	/*gl_Position = MVPMatrix * gl_in[0].gl_Position;
	EmitVertex;
	
	gl_Position = MVPMatrix * gl_in[1].gl_Position;
	EmitVertex;

	EndPrimitive();*/
}

)"