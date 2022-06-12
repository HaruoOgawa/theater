
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
uniform float _isWhiteOut;
uniform float _whiteOutVal;

#define rot(a) mat2(cos(a),sin(a),-sin(a),cos(a))
#define PI 3.14159265

struct DBuffer{
  float d1;
  float d2;
  float d3;
  float d4;
  float d5;
  float mainD;
};

struct AccBuffer{
  float acc1;
  float acc2;
  float acc3;
  float acc4;
  float acc5;
};

float rand(vec2 seeds){
  return fract(sin(dot(seeds, vec2(12.9898,78.233))) * 43758.5453);
}

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

float Plane(vec3 p){
  float d=-p.y+1.0;
  d=min(d,p.y+1.0);

  return d;
}

float Cube(vec3 p,vec3 s){
  return length(max(abs(p)-s,0.0));
}

float d1(vec3 p){
  p.z-=time;
  p.y+=perlinNoise(vec2(p.x,p.z))+perlinNoise(vec2(p.x,p.z)*5.0)*0.5+perlinNoise(vec2(p.x,p.z)*10.0)*0.35;
  p.y*=0.6;
  p.y=p.y*2.0-1.0;
  float d=Plane(p);
      return d;
}

float d2(vec3 p){
p.z-=time*10.;
  vec3 pos0=p;
  float r=perlinNoise(vec2(pos0.x,pos0.z))+perlinNoise(vec2(pos0.x,pos0.y)*5.0)*0.5+perlinNoise(vec2(pos0.y,pos0.z)*10.0)*0.35;
float k=2.1;
pos0=mod(pos0,k)-k*0.5;

  float d=length(pos0)-r;

  return d;
}


float d3(vec3 p){
//p*=7.;
  p+=vec3(0.,0.,1.);

  float d=length(p)-.1;
//p.xyz*=7.;
//p.x+=1.5;

  p.xy*=rot(time);
  p.xz*=rot(time);
  p.yz*=rot(time);
  for(int i=0;i<3;i++){
    p=abs(p)-.035;
    if(p.x<p.y)p.xy=p.yx;
    if(p.x<p.z)p.xz=p.zx;
    if(p.y<p.z)p.yz=p.zy;

    p.xy*=rot(1.2);
    p.xz*=rot(.25);
    p.yz*=rot(.25);

}

d=min(d,Cube(p,vec3(.2,.2,.2)*.1));

  return d;
}

float d4(vec3 p){

    p+=vec3(0.,0.,-1.);
    p.xz*=rot(time);

    p*=0.5;
    for(int i=0;i<3;i++){
      p=abs(p)-.02;
      if(p.x<p.y)p.xy=p.yx;
      if(p.x<p.z)p.xz=p.zx;
      if(p.y<p.z)p.yz=p.zy;

      p.xy*=rot(length(p)-2.5);
      p.xz*=rot(0.2);
      p.yz*=rot(length(p)-2.25);
    }
    float h=1.2;
    p.x-=clamp(p.x,-h,h);


float d=Cube(p,vec3(.1,.1,.1)*0.1);
  return d;
}

float Lighting(vec3 p,float timeOffset,float randRot){
  p.xy*=rot(randRot*2.0*PI*0.5);
  p.yz*=rot(randRot*2.0*PI);
  p.y+=perlinNoise(vec2(p.z*15.,p.z*10.)*5.)*0.1;
  float d=Cube(p,vec3(.002,.002,1.)*2.)*((p.z>mod(time*4.0+timeOffset,3.))? 1.0:2000.0);
  return d;
}

float d5(vec3 p){
  p+=vec3(0.,0.,1.);

  float d=Lighting(p,0.0,0.0);
  for(int i=0;i<5;i++){
    vec3 pos=p;
    float r=rand(vec2(float(i),0.456));
    r=r*0.25+0.25;
    r*=0.5;
    pos.xy+=vec2(
      r*cos(2.0*PI*rand(vec2(float(i),0.123))),
      r*sin(2.0*PI*rand(vec2(float(i),0.456)))
      );
    d=min(d,Lighting(pos,rand(vec2(0.963,float(i))),rand(vec2(0.741,float(i)))));
  }

  return d;
}

DBuffer map(vec3 p){
  DBuffer d;
  d.d1=d1(p);
  //d.d1=2000.0;
  d.d2=d2(p);
  //d.d2=d2(p);
//  d.d3=2000.0;
d.d3=d3(p);
d.d4=d4(p);
//d.d4=2000.0;
d.d5=d5(p);
//d.d5=2000.0;
d.mainD=min(min(min(d.d1,d.d2),min(d.d3,d.d4)),d.d5);
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
//st*=rot(time);
  vec3 col=vec3((st.y+1.0)*0.5*0.25);
  float radius=2.5;
float speed=-.25;
 vec3 ta=vec3(0.);
 float adjustAcc=0.5;
// vec3 ro=vec3(-1.,0.,0.);
// vec3 ro=vec3(-1.5,0.,1.5);
vec3 ro=vec3(cos(time)*radius,0.0,sin(time)*radius);

if(_cameraPositionIndex==0.0){
    ro=vec3(cos(time)*radius,0.0,sin(time)*radius);
}else if(_cameraPositionIndex==1.0){
    ro=vec3(-2.,0.,-2.)*3.0;
}else if(_cameraPositionIndex==2.0){
    float radius=1.0*(perlinNoise(vec2(0.123,time*5.0))+1.0)*2.25;
    ro=vec3(cos(time)*radius,sin(time)*radius*0.1,sin(time)*radius);
    //ro=vec3(-1.5,0.,1.5)
}

 vec3 cDir=normalize(ta-ro);
 vec3 cSide=normalize(cross(cDir,vec3(0.,-1.,0.)));
 vec3 cUp=normalize(cross(cDir,cSide));
 float depth=1.;
 vec3 rd=normalize(vec3(st.x*cSide+st.y*cUp+cDir*depth));

  DBuffer d;
  AccBuffer acc;
  float t=0.0;
  for(int i=0;i<128;i++){
    d=map(ro+rd*t);
    if(d.d1<0.001||d.d2<0.001||d.d3<0.001||d.d4<0.001||d.d5<0.001||t>1000.0)break;
    t+=d.mainD*.5;

    acc.acc1+=exp(-50.0*(d.d1));
    acc.acc2+=exp(-50.0*d.d2);
    acc.acc3+=exp(-50.0*d.d3);
    acc.acc4+=exp(-50.0*d.d4);
    acc.acc5+=exp(-50.0*d.d5);
  }

    if(d.d1<0.001){
      vec3 pos=ro+rd*t;
      float flash=1.0-abs(sin(pos.z*.5+time*4.0));
      flash+=.25;
    col+=vec3(exp(-.1*t))*vec3(0.,0.25,1.0)*acc.acc1*0.05*flash;
    }

    if(d.d2<0.001){
      vec3 pos=ro+rd*t;
      float flash=1.0-abs(sin(pos.z*.5+time*4.0));
      flash+=.25;
      col+=vec3(exp(-.01*t))*vec3(0.,0.25,1.0)*acc.acc2*0.04*flash;
    }

    if(d.d3<0.001){
        col+=exp(-.025*t)*acc.acc3*0.02*vec3(0.5,0.25,1.0);
    }

    if(d.d4<0.001){
      vec3 refro=ro+rd*t;
      vec3 n=gn(refro);
      rd=refract(rd,n,0.786);
      ro=refro;
      t=0.1;
      float acc2=0.0;

      for(int i=0;i<33;i++){
        d=map(ro+rd*t);
        if(d.d1<0.001||d.d2<0.001||d.d3<0.001||d.d4<0.001||t>1000.0)break;
        t+=d.mainD;
        acc2+=exp(-3.*d.mainD);
      }


      vec3 pos=ro+rd*t;
      float flash=1.0-abs(sin(pos.z*.5+time*4.0));
      flash+=.1;
      float H = mod(time*0.5, 1.0);
      col+=vec3(exp(-.1*t))*acc2*.25*flash*hsv2rgb2(vec3(H,1.0,1.0),2.2)*adjustAcc;
    }

    if(d.d5<0.01){
      //col+=vec3(-.1*t)*acc.acc5*0.075;
      col+=vec3(1.)*acc.acc5*0.025*vec3(1.,1.,1.);
    }

    if(_isWhiteOut==1.0){
        col+=_whiteOutVal*vec3(1.0);
    }

  gl_FragColor = vec4(col, 1.0);
}
