precision mediump float;
/*
uniform float time;
uniform vec2 resolution;
uniform vec2 uv;
*/
in vec2 uv;
in float time;
in vec2 resolution;

uniform float _cameraPositionIndex;

#define rot(a) mat2(cos(a),sin(a),-sin(a),cos(a))
#define PI 3.14159265
struct DBuffer{
  float d1;
  float d2;
  float d3;
  float mainD;
};

struct AccBuffer{
  float acc1;
  float acc2;
  float acc3;
};

vec2 random2(vec2 seeds)
{
 seeds = vec2(dot(seeds, vec2(127.1, 311.7)),
 dot(seeds, vec2(269.5, 183.3)));
 return fract(sin(seeds) * 43758.5453123);
}

float perlinNoise(vec2 seeds)
{
vec2 i = floor(seeds);
vec2 f = fract(seeds);
vec2 i00 = i + vec2(0, 0);
vec2 i10 = i + vec2(1, 0);
vec2 i01 = i + vec2(0, 1);
vec2 i11 = i + vec2(1, 1);
vec2 f00 = f - vec2(0, 0);
vec2 f10 = f - vec2(1, 0);
vec2 f01 = f - vec2(0, 1);
vec2 f11 = f - vec2(1, 1);
vec2 g00 = normalize(-1.0 + 2.0 * random2(i00));
vec2 g10 = normalize(-1.0 + 2.0 * random2(i10));
vec2 g01 = normalize(-1.0 + 2.0* random2(i01));
vec2 g11 = normalize(-1.0 + 2.0* random2(i11));
float v00 = dot(g00, f00);
float v10 = dot(g10, f10);
float v01 = dot(g01, f01);
float v11 = dot(g11, f11);
vec2 p = smoothstep(0.0, 1.0, f);
float v00v10 = mix(v00, v10, p.x);
float v01v11 = mix(v01, v11, p.x);
return mix(v00v10, v01v11, p.y) * 0.5 + 0.5;
}

vec2 fmod(vec2 p,float r){
  float a=atan(p.x,p.y)+PI/r;
  float n=(2.*PI)/r;
  a=floor(a/n)*n;
  return rot(a)*p;
}


#define foldingLimit 1.0
vec3 boxFold(vec3 z, float dz) {
    return clamp(z, -foldingLimit, foldingLimit) * 2.0 - z;
}

void sphereFold(inout vec3 z, inout float dz, float minRadius, float fixedRadius) {
    float m2 = minRadius * minRadius;
    float f2 = fixedRadius * fixedRadius;
    float r2 = dot(z, z);
    if (r2 < m2) {
        float temp = (f2 / m2);
        z *= temp;
        dz *= temp;
    } else if (r2 < f2) {
        float temp = (f2 / r2);
        z *= temp;
        dz *= temp;
    }
}

// ref: http://blog.hvidtfeldts.net/index.php/2011/11/distance-estimated-3d-fractals-vi-the-mandelbox/
#define ITERATIONS 12
float deMandelbox(vec3 p, float scale, float minRadius, float fixedRadius) {
    vec3 z = p;
    float dr = 1.;
    for (int i = 0; i < ITERATIONS; i++) {
        z = boxFold(z, dr);
        sphereFold(z, dr, minRadius, fixedRadius);
        z = scale * z + p;
        dr = dr * abs(scale) + 1.;
    }
    float r = length(z);
    return r / abs(dr);
}

float Plane(vec3 p){
  return p.y;
}

float Cube(vec3 p,vec3 s){
  return length(max(abs(p)-s,0.0));
}

float d1(vec3 p){
     p*=.5;

/*  p.xy*=rot(time);
  p.xz*=rot(time);
  p.yz*=rot(time);*/

float speed=10.0;
  p.z-=time*speed;
  float k=12.0;
  p=mod(p,k)-k*0.5;
  
  p=abs(p);
  float s=1.;
  float ts=0.05;
  float dt=floor(time)+pow(fract(time),0.5);
  for(int i=0;i<2;i++){
    p.xy=fmod(p.xy,12.);
    p.xz*=rot(0.25);
    p.xz=fmod(p.xz,6.);
    p.yz*=rot(0.25);
    p.yz=fmod(p.yz,6.);
    p.xy*=rot(0.25);

   p.x=abs(p.x)-0.5;
    p.z=abs(p.z)-.015;
  }

  p.xy=fmod(p.xy,36.);

  float val=sin(.15);
  float d=deMandelbox(p, 2.0, .05+val, 1.+val*0.05);
  return d;
  //return length(p)-0.5;
}

float d2(vec3 p){
   

  float d=length(p)-1.;

//p.x+=1.5;
  p.xy*=rot(time);
  p.xz*=rot(time);
  p.yz*=rot(time);
  for(int i=0;i<3;i++){
    p=abs(p)-.6;
    if(p.x<p.y)p.xy=p.yx;
    if(p.x<p.z)p.xz=p.zx;
    if(p.y<p.z)p.yz=p.zy;

    p.xy*=rot(0.5);
    p.xz*=rot(0.15);
    p.yz*=rot(0.25);
}

d=min(d,Cube(p,vec3(.2,.2,.2)));

  return d;
}
DBuffer map(vec3 p){
  DBuffer d;
  d.d1=d1(p);
  d.d2=d2(p-vec3(0.,0.,9.));
  d.d3=2000.0;
  d.mainD=min(min(d.d1,d.d2),d.d3);
  return d;
}

vec3 gn(vec3 p){
  vec2 e=vec2(0.001,0.);
  return normalize(
    vec3(
      map(p+e.xyy).mainD-map(p-e.xyy).mainD,
      map(p+e.yxy).mainD-map(p-e.yxy).mainD,
      map(p+e.yyx).mainD-map(p-e.yyx).mainD
      )
    );
}

vec3 hsv2rgb2(vec3 c, float k) {
    return smoothstep(0. + k, 1. - k,
        .5 + .5 * cos((vec3(c.x, c.x, c.x) + vec3(3., 2., 1.) / 3.) * radians(360.)));
}

void main(void) {
  //vec2 st = (gl_FragCoord.xy*2.0-resolution.xy) / min(resolution.x,resolution.y);
  vec2 st=uv*2.0-1.0;
  st.x*=(resolution.x/resolution.y);

  vec3 col=vec3((st.y+1.0)*0.5*0.25);
  float radius=15.;
float speed=-.25;
 vec3 ta=vec3(0.);
// vec3 ta=vec3(cos(time),0.,sin(time));
 vec3 ro=vec3(0.,0.,14.);

 float adjustAcc=0.5;
 if(_cameraPositionIndex==0){
 adjustAcc=0.5;
    ro=vec3(0.,0.,15.);
 }else if(_cameraPositionIndex==1){
    adjustAcc=0.5;
    float radius=1.0*(perlinNoise(vec2(0.123,time*5.0))+1.0)*2.25;
    ta=vec3(0.,0.,9.);
    ro=vec3(cos(time)*radius,sin(time)*radius,sin(time)*radius);
 }else if(_cameraPositionIndex==2){
    adjustAcc=0.25;
    //st*=rot(time);
    ro=vec3(-2.,-1.,20.);
 }

// vec3 ro=vec3(0.,0.,0.);
 vec3 cDir=normalize(ta-ro);
 vec3 cSide=cross(cDir,vec3(0.,-1.,0.));
 vec3 cUp=cross(cDir,cSide);
 float depth=1.;
 vec3 rd=normalize(vec3(st.x*cSide+st.y*cUp+cDir*depth));

  DBuffer d;
  AccBuffer acc;
  float t=0.0;
  for(int i=0;i<128;i++){
    d=map(ro+rd*t);
    if(d.d1<0.01||d.d2<0.001||d.d3<0.001||t>1000.0)break;
    t+=d.mainD;

    acc.acc1+=exp(-6.0*(d.mainD));
    acc.acc2+=exp(-1.0*d.mainD);
    acc.acc3+=exp(-1.0*d.mainD);
  }

  if(d.d1<0.01||t<1.0){
    vec3 pos=ro+rd*t;
    float flash=1.0-abs(sin(pos.z*.5+time*4.0));
    flash+=.1;
    float rim=pow(min(d.d1,2.0),.5);
    float H = mod(time*0.5, 1.0);
    col+=vec3(exp(-.1*t))*rim*acc.acc1*.75*flash*hsv2rgb2(vec3(H,1.0,1.0),2.2)*(1.0/(adjustAcc*2.0));
    //*pow((1.0-abs(dot(n,rd))),.5)
    ;
  }

  if(d.d2<0.001){
    vec3 refro=ro+rd*t;
    vec3 n=gn(refro);
    rd=reflect(rd,n);
    ro=refro;
    t=0.1;
    float acc2;

    for(int i=0;i<33;i++){
      d=map(ro+rd*t);
      if(d.mainD<0.001)break;
      t+=d.mainD;
      float H = mod(time*0.5, 1.0);
      acc2+=exp(-3.*d.mainD);
    }

    vec3 pos=ro+rd*t;
    float flash=1.0-abs(sin(pos.z*.5+time*4.0));
    flash+=.1;
    float H = mod(time*0.5, 1.0);
    col+=vec3(exp(-.1*t))*acc.acc2*.75*flash*hsv2rgb2(vec3(H,1.0,1.0),2.2)*adjustAcc;
  }

  gl_FragColor = vec4(col, 1.0);
}
