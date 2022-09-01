R"(
#version 330

uniform float _time;
uniform vec2 _resolution;
uniform float _RenderingTarget;
uniform vec3 _WorldCameraPos;

in vec2 uv;

#define PI 3.14159265
#define rot(a) mat2(cos(a),sin(a),-sin(a),cos(a))

float rand(vec2 st)
{
    return fract(sin(dot(st, vec2(12.9898, 78.233))) * 43758.5453);
}

 float PerlinNoise(float x) {
                float aL = rand(vec2(floor(x), 0.1));
                float aR = rand(vec2(floor(x + 1.0), 0.1));
                float wL = aL * fract(x);
                float wR = aR * (fract(x) - 1.0);
                float f = fract(x);
                float u = pow(f, 2.0) * (3.0 - 2.0 * f);
                float n = mix(wL, wR, u);

                return n;
}

float smin(float d1,float d2,float v)
{
    float h=exp(-v*d1)+exp(-v*d2);
    return -log(h)/v;
}

float sp(vec3 p,float r)
{
    return length(p)-r;
}

#define count 6
float map(vec3 p)
{

    float d=10.0;
    float tm=_time*0.75;
    for(int c=0;c<count;c++)
    {
        vec3 pos=p;
        float id=float(c);
        float r=clamp(rand(vec2(id,0.456))*0.5,0.25,0.5);
        
        vec3 o=vec3(0.);
        o.x+=PerlinNoise(tm+rand(vec2(id,0.159))*10.0)*8.0;
        o.y+=PerlinNoise(tm+rand(vec2(id,0.951))*10.0)*8.0;
        o.z+=PerlinNoise(tm+rand(vec2(id,0.357))*10.0)*8.0;
        d=smin(d,sp(pos+o,0.25),2.);
    }
    return d;
}

vec3 gn(vec3 p)
{
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
  vec3 ta=vec3(0.0);
  
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
    acc+=exp(-3.0*d);
  }
 
  if(abs(d)<dmin)
  {
    if(_RenderingTarget==2.0) // ZTest
    {
        vec3 col=vec3(1.0)*0.005/pi;
        gl_FragColor=vec4(col,1.0);
    }
    else
    {
        //vec3 col=vec3(1.0)*10./pi;
        vec3 col=acc*0.05*vec3(1.0);
        //vec3 col=vec3(1.0);
        gl_FragColor=vec4(col,1.0);
    }
  }
  else
  {
    if(_RenderingTarget==2.0)
    {
        //vec3 col=vec3(1.0);
        vec4 col=vec4(0.0);
        gl_FragColor=col;
    }
  }

}

)"