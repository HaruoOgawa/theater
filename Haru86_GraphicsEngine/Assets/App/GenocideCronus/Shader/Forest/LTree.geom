R"(

#version 410

#define PI 3.14159265

layout(lines) in;
layout(triangle_strip,max_vertices =128) out;

in vec3 LocalNormal[];
in float v2g_treeRadius[];
flat in float v2g_id[];

uniform mat4 MVPMatrix;
uniform mat4 MMatrix;

// Tube Uniform
uniform float _TreeMaxRadius;
uniform int _TreeSegment;

out vec3 WorldVertexPos;
out vec3 WorldNormal;
flat out float g2f_id;

void main()
{
	vec4 pos0 = gl_in[0].gl_Position;
	vec4 pos1 = gl_in[1].gl_Position;

	vec3 tangent = normalize(pos1.xyz-pos0.xyz);
	vec3 normal	 = normalize(LocalNormal[0]);
	vec3 bionormal = normalize(cross(tangent,normal));

	float angle = 2.0 * PI / float(_TreeSegment);

	// ‚±‚±‚ÌŒvŽZ‚Í‚½‚Ô‚ñŒã‚ÅŒ©’¼‚·•K—v‚ª‚ ‚é >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
	//int LoopNum = _TreeSegment + 1;
	int LoopNum = 3 + 1;
	for(int n=0; n < LoopNum; n++)
	{
		float theta0 =  angle * (float(n));
		vec3 offVec0 = v2g_treeRadius[0] * normalize(cos(theta0)*bionormal + sin(theta0)*normal);

		float theta1 =  angle * (1.0 + float(n));
		vec3 offVec1 = v2g_treeRadius[0] * normalize(cos(theta1)*bionormal + sin(theta1)*normal);

		// triangle0
		gl_Position = MVPMatrix * vec4(pos0.xyz+offVec0,1.0);
		WorldVertexPos = (MMatrix * vec4(pos0.xyz+offVec0,1.0)).xyz;
		WorldNormal = ( vec4(normalize(offVec0),0.0)).xyz;
		g2f_id = v2g_id[0];
		EmitVertex();

		gl_Position = MVPMatrix * vec4(pos0.xyz+offVec1,1.0);
		WorldVertexPos = (MMatrix * vec4(pos0.xyz+offVec1,1.0)).xyz;
		WorldNormal = ( vec4(normalize(offVec1),0.0)).xyz;
		g2f_id = v2g_id[0];
		EmitVertex();

		gl_Position = MVPMatrix * vec4(pos1.xyz+offVec1,1.0);
		WorldVertexPos = (MMatrix * vec4(pos0.xyz+offVec1,1.0)).xyz;
		WorldNormal = ( vec4(normalize(offVec1),0.0)).xyz;
		g2f_id = v2g_id[0];
		EmitVertex();

		EndPrimitive();

		// triangle1
		gl_Position = MVPMatrix * vec4(pos0.xyz+offVec0,1.0);
		WorldVertexPos = (MMatrix * vec4(pos0.xyz+offVec0,1.0)).xyz;
		WorldNormal = ( vec4(normalize(offVec0),0.0)).xyz;
		g2f_id = v2g_id[0];
		EmitVertex();

		gl_Position = MVPMatrix * vec4(pos1.xyz+offVec1,1.0);
		WorldVertexPos = (MMatrix * vec4(pos0.xyz+offVec1,1.0)).xyz;
		WorldNormal = ( vec4(normalize(offVec1),0.0)).xyz;
		g2f_id = v2g_id[0];
		EmitVertex();

		gl_Position = MVPMatrix * vec4(pos1.xyz+offVec0,1.0);
		WorldVertexPos = (MMatrix * vec4(pos0.xyz+offVec0,1.0)).xyz;
		WorldNormal = ( vec4(normalize(offVec0),0.0)).xyz;
		g2f_id = v2g_id[0];
		EmitVertex();

		EndPrimitive();

		// ã‚ÌŠW
		gl_Position = MVPMatrix * vec4(pos1.xyz,1.0);
		WorldVertexPos = (MMatrix * vec4(pos0.xyz,1.0)).xyz;
		WorldNormal = ( vec4(normalize(tangent),0.0)).xyz;
		g2f_id = v2g_id[0];
		EmitVertex();

		gl_Position = MVPMatrix * vec4(pos1.xyz+offVec1,1.0);
		WorldVertexPos = (MMatrix * vec4(pos0.xyz+offVec1,1.0)).xyz;
		WorldNormal = ( vec4(normalize(tangent),0.0)).xyz;
		g2f_id = v2g_id[0];
		EmitVertex();

		gl_Position = MVPMatrix * vec4(pos1.xyz+offVec0,1.0);
		WorldVertexPos = (MMatrix * vec4(pos0.xyz+offVec0,1.0)).xyz;
		WorldNormal = ( vec4(normalize(tangent),0.0)).xyz;
		g2f_id = v2g_id[0];
		EmitVertex();

		EndPrimitive();

		// ‰º‚ÌŠW
		gl_Position = MVPMatrix * vec4(pos0.xyz,1.0);
		WorldVertexPos = (MMatrix * vec4(pos0.xyz,1.0)).xyz;
		WorldNormal = ( vec4(normalize(tangent),0.0)).xyz;
		g2f_id = v2g_id[0];
		EmitVertex();

		gl_Position = MVPMatrix * vec4(pos0.xyz+offVec1,1.0);
		WorldVertexPos = (MMatrix * vec4(pos0.xyz+offVec1,1.0)).xyz;
		WorldNormal = ( vec4(normalize(tangent),0.0)).xyz;
		g2f_id = v2g_id[0];
		EmitVertex();

		gl_Position = MVPMatrix * vec4(pos0.xyz+offVec0,1.0);
		WorldVertexPos = (MMatrix * vec4(pos0.xyz+offVec0,1.0)).xyz;
		WorldNormal = ( vec4(normalize(tangent),0.0)).xyz;
		g2f_id = v2g_id[0];
		EmitVertex();

		EndPrimitive();
	}
}

)"