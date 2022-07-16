R"(

#version 410

layout(triangles) in;
layout(triangle_strip,max_vertices=3) out;

uniform mat4 MVPMatrix;
uniform mat4 MMatrix;
uniform mat4 VMatrix;
uniform mat4 PMatrix;
uniform float _time;
uniform float _deltaTime;

in vec2 e2g_uv[];

out vec3 g2f_normal;
out vec2 g2f_uv;

void main(){
    vec4 p0=gl_in[0].gl_Position;
    vec4 p1=gl_in[1].gl_Position;
    vec4 p2=gl_in[2].gl_Position;

    ///vec3 normal=normalize(cross(normalize(p1.xyz-p0.xyz),normalize(p2.xyz-p0.xyz)));
    vec3 normal=cross(p1.xyz-p0.xyz,p2.xyz-p0.xyz);
    vec3 center=(p0.xyz+p1.xyz+p2.xyz)/3.0;

    vec3 n0=normalize(normal+(p0.xyz-center));
    vec3 n1=normalize(normal+(p1.xyz-center));
    vec3 n2=normalize(normal+(p2.xyz-center));

    gl_Position =MVPMatrix*p0;
    g2f_normal=(MMatrix*vec4(n0,1.0)).xyz;
    g2f_uv=e2g_uv[0];
    EmitVertex();

    gl_Position =MVPMatrix*p1;
    g2f_normal=(MMatrix*vec4(n1,1.0)).xyz;
    g2f_uv=e2g_uv[1];
    EmitVertex();
    
    gl_Position =MVPMatrix*p2;
    g2f_normal=(MMatrix*vec4(n2,1.0)).xyz;
    g2f_uv=e2g_uv[2];
    EmitVertex();
    
    EndPrimitive();
}

)"