R"(
#version 330

uniform float _time;
uniform vec2 _resolution;
uniform float _RenderingTarget;
uniform vec3 _WorldCameraPos;
uniform vec3 _WorldCameraCenter;
uniform int _IsDrawMandel;

in vec2 uv;

#define rot(a) mat2(cos(a),sin(a),-sin(a),cos(a))
#define PI 3.14159265

mat3 orthBas(vec3 z){
  z=normalize(z);
  vec3 up=abs(z.y) > 0.999 ? vec3(0,0,1) : vec3(0,1,0);
  vec3 x=normalize(cross(up,z));
  return mat3(x,cross(z,x),z);
}

vec3 cyclicNoise(vec3 p){
  mat3 b=orthBas(vec3(3.0,-1.2,5.4));
  float warp=1.0;
  float amp=0.5;

  vec3 result=vec3(0.0);

  for(int i=0;i<4;i++){
    p*=2.0*b;
    p+=warp*sin(p.zxy);

    result+=amp*cross(sin(p.yzx),cos(p));

    warp*=1.2;
    amp*=0.5;
  }

  return result;
}

void SphereFold(inout vec3 z,inout float dz,float minRadius,float fixedRadius)
{
    float m2=minRadius*minRadius;
    float f2=fixedRadius*fixedRadius;
    float r2=dot(z,z);
    if(r2<m2)
    {
        float temp=(f2/m2);
        z*=temp;
        dz*=temp;
    }
    else if(r2<f2)
    {
        float temp=(f2/r2);
        z*=temp;
        dz*=temp;
    }
}

vec3 boxFold(vec3 z, float dz) {
    return clamp(z, -1., 1.) * 2.0 - z;
}

float map(vec3 p){
    
    // ˆÚ“®
    p.y+=3.0; 
    
    // ‰ñ“]
    p.xz*=rot(PI/4.0);

   /* p.xy*=rot(PI/4.0);
    p.yz*=rot(PI/6.0);
    p.xz*=rot(PI/4.0);*/
    
    /*p.xy*=rot(_time*0.1);
    p.yz*=rot(_time*0.1);
    p.xz*=rot(_time*0.1);*/
    
    // Šg‘åk¬
    p*=2.0;

    float scale=2.0;
    vec3 z = p;
    float dr = 1.0;
    for (int i = 0; i < 12; i++) {
        z=boxFold(z,dr);
        SphereFold(z, dr, 0.1, 1.0);
        z = scale * z + p;
        dr = dr * abs(scale) + 1.0;
    }
    float r = length(z);
    return r / abs(dr);
}

vec3 gn(vec3 p){
  vec2 e=vec2(0.001,0.0);
  return normalize(vec3(
      map(p+e.xyy)-map(p-e.xyy),
      map(p+e.yxy)-map(p-e.yxy),
      map(p+e.yyx)-map(p-e.yyx)
    ));
}

void main(){
  //vec2 st=(fragCoord.xy*2.-_resolution.xy)/min(_resolution.x,_resolution.y);
  vec2 st=uv*2.0-1.0;
  st.x*=(_resolution.x/_resolution.y);

  //vec3 ro=vec3(0.0,0.0,15.0);
  vec3 ro=_WorldCameraPos;
  vec3 ta=_WorldCameraCenter;
  
  vec3 cdir=normalize(ta-ro);
  vec3 cside=normalize(cross(cdir,vec3(0.0,1.0,0.0)));
  vec3 cup=normalize(cross(cdir,cside));
  
  vec3 rd=normalize(st.x*cside+st.y*cup+cdir*1.0);

  float d,t,acc=0.0,pi=0.0,dmin=0.01;
  for(int i=0;i<128;i++){
    d=map(ro+rd*t);
    pi=float(i);
    if(abs(d)<dmin||t>100.0)break;
    t+=d;
  }
 
  if(abs(d)<dmin)
  {
    if(_RenderingTarget==2.0) // ZTest
    {
        vec3 col=vec3(1.0)*1./pi;
        gl_FragColor=vec4(col,1.0);
    }
    else
    {
        if(_IsDrawMandel==1)
        {
             vec3 col=vec3(1.0)*10./pi;
            gl_FragColor=vec4(col,1.0);
        }
    } 
  }  
  else
  {
       vec3 col=cyclicNoise(vec3(st*5.,_time));
       col=vec3((col.r+col.g+col.b)*0.333);
       gl_FragColor=vec4(col,1);
  } 

}

)"