
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
uniform float isTransform;

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
  //float d=-p.y+1.0;
  float d=p.y+1.0;

  return d;
}

float Cube(vec3 p,vec3 s){
  return length(max(abs(p)-s,0.0));
}

vec2 pmod(vec2 p, float n)
{
float a = atan(p.x, p.y) + PI / n;
float th = 2.0 * PI / n;
a = floor(a / th) * th;
return rot(a)* p;
}

float Cube(vec3 p, float s)
{
p = abs(p);
return length(max(p - vec3(s, s, s), 0.0));
}

float sdCross(vec3 p, float scale)
{
p = abs(p);
float dxy = max(p.x, p.y);
float dyz = max(p.y, p.z);
float dzx = max(p.z, p.x);
return min(dxy, min(dyz, dzx)) - scale;
}


float menger(vec3 p)
{
    float k = 1.;
    p.z = mod(p.z, k) - 0.5 * k;

    //
    float s = .8;
    p = abs(p);
    float d = Cube(p, s);
    //
    float scale = 4.;
    //
    float h = 1.5;
    for (int i = 0; i < 4; i++)
    {

        p = mod(p, h) - h * 0.5;
        s *= scale;
        p = 1.0 - scale * abs(p);
        //
        d = max(d, sdCross(p, .79) / s);

    }

    return d;
}


float d1(vec3 p){
  float d=Plane(p);
      return d;
}

float d2(vec3 p){
  vec3 pos = p;
   pos.yz*=rot(PI/2.0);
  pos=abs(pos)-0.5;
  pos.z-=0.5;
  pos.z-=0.2;
  pos.z-=0.5;
  pos.z-=0.1;

  pos.xy = pmod(pos.xy, 10.0);
  float k=1.2;
  pos=mod(pos,k)-k*0.5;
  float d = menger(pos);


  return d;
}

float d3(vec3 p){
  //float isTransform=1.0;

  if(isTransform==0.0){
    p+=vec3(0.,.9,0.);
    p.xz*=rot(time);
    p.xy*=rot(time);
    p.yz*=rot(time);
  }else if(isTransform==1.0){
      p+=vec3(-.15,.75,-.15);
      p.xz*=rot(time);
  }

  if(isTransform==1.0){
    p*=0.75;
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
  }

float d=Cube(p,vec3(.1,.1,.1)*0.1);
  return d;
}


float d4(vec3 p){
  p*=7.5;

    p+=vec3(1.,7.,1.);
    float d=length(p)-.2;
  //p.xyz*=7.;
  //p.x+=1.5;

    p.xy*=rot(time);
    p.xz*=rot(time);
    p.yz*=rot(time);
    for(int i=0;i<3;i++){
      p=abs(p)-.075;
      if(p.x<p.y)p.xy=p.yx;
      if(p.x<p.z)p.xz=p.zx;
      if(p.y<p.z)p.yz=p.zy;

      p.xy*=rot(1.2);
      p.xz*=rot(.25);
      p.yz*=rot(.25);

  }

  d=min(d,Cube(p,vec3(.2,.2,.2)*.2));

    return d;
}

float Lighting(vec3 p,float timeOffset,float randRot){
  p.xz*=rot(randRot*2.0*PI);
  p.x+=perlinNoise(vec2(p.y*15.,p.y*10.)*5.)*0.05;
  float d=Cube(p,vec3(.00005,.5,.00005))*((p.y>mod(time*4.0+timeOffset,0.95*1.))? 1.0:2000.0);
  return d;
}

float d5(vec3 p){
  p+=vec3(0.,1.1,0.);

  float d=Lighting(p,0.0,0.0);
  for(int i=0;i<30;i++){
    vec3 pos=p;
    float r=rand(vec2(float(i),0.456));
    r=r*0.5+0.25;
    pos.xz+=vec2(
      r*cos(2.0*PI*rand(vec2(float(i),floor(time*2.0)))),
      r*sin(2.0*PI*rand(vec2(float(i),floor(time*2.0))))
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
  //d.d2=2000.0;
  d.d3=d3(p);
  //d.d3=2000.0;
  d.d4=d4(p);
  //d.d4=2000.0;

  if(isTransform==0.0){
   d.d5=2000.0;
  }else if(isTransform==1.0){
      d.d5=d5(p);
  }

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

  vec3 col=vec3((st.y+1.0)*0.5*0.25);

float speed=-.25;
 vec3 ta=vec3(0.,-0.95,0.);

 float adjustAcc=0.5;
float radius=.3;
 radius=.25;
 vec3 ro=vec3(-radius,-0.8,-radius);

    float adjustSpeed=0.25;
 if(isTransform==0.0){
    if(_cameraPositionIndex==0.0){
        radius=.3;
        ro=vec3(cos(time*adjustSpeed)*radius,-0.8,sin(time*adjustSpeed)*radius);
    }else if(_cameraPositionIndex==1.0){
        ta=vec3(0.,-.9,0.);
        radius=.025;
        ro=ta+vec3(cos(time*adjustSpeed)*radius,0.0,sin(time*adjustSpeed)*radius);
    }else if(_cameraPositionIndex==2.0){
        radius=.3;
        ro=vec3(cos(time*adjustSpeed)*radius,-0.3,sin(time*adjustSpeed)*radius);
    }
 }else if(isTransform==1.0){
     if(_cameraPositionIndex==0.0){
        radius=.25;
        ro=vec3(-radius,-0.8,-radius);
    }else if(_cameraPositionIndex==1.0){
        radius=.25;
        ro=vec3(-radius,-0.8,0.);
    }else if(_cameraPositionIndex==2.0){
        radius=.25;
        ro=vec3(-radius,-0.4,-radius);
    }
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
    if(d.d1<0.001||d.d2<0.001||d.d3<0.001||d.d4<0.001||d.d5<0.001||d.mainD<0.001||t>1000.0)break;
    t+=d.mainD*.5;

    acc.acc1+=exp(-10.0*(d.d1));
    acc.acc2+=exp(-6.0*d.d2);
    acc.acc3+=exp(-50.0*d.d3);
    acc.acc4+=exp(-50.0*d.d4);
    acc.acc5+=exp(-3.0*d.d5);
  }

    if(d.d1<0.01){
      vec3 pos=ro+rd*t;
    float xid=floor(pos.x*20.0);
    float zid=floor(pos.z*20.0);
    col+=vec3(exp(-.5*t))*(
      (
        fract((xid+zid)/2.0)==0.0
        )? vec3(1.):vec3(0.25)
      )*vec3(0.5,0.25,0.25);
    }

    if(d.d2<0.001){
      col+=vec3(exp(-.5*t))*vec3(0.5,0.25,0.25)*acc.acc2*0.02;
    }

    //col=vec3(1.);

    if(d.d3<0.001){
      vec3 refro=ro+rd*t;
      vec3 n=gn(refro);
      rd=refract(rd,n,0.786);
      ro=refro;
      t=0.1;
      float acc2=0.0;

      for(int i=0;i<33;i++){
        d=map(ro+rd*t);
        if(d.d1<0.001||d.d2<0.001||d.d3<0.001||t>1000.0)break;
        t+=d.mainD;
        acc2+=exp(-3.*d.mainD);
      }


      vec3 pos=ro+rd*t;
      float flash=1.0-abs(sin(pos.z*.5+time*4.0));
      flash+=.1;
      float H = mod(time*0.5, 1.0);
      col+=vec3(exp(-.1*t))*acc2*.25*flash*hsv2rgb2(vec3(H,1.0,1.0),2.2)*adjustAcc;
    }

    if(d.d4<0.01){
      col+=exp(-.025*t)*acc.acc4*0.2*vec3(0.5,0.25,1.0);
    }

    if(d.d5<0.01){
      //col+=vec3(-.1*t)*acc.acc5*0.075;
      col+=vec3(1.)*acc.acc5*0.05*vec3(0.5,0.5,1.);
    }

    //col=vec3(0.);

  gl_FragColor = vec4(col, 1.0);
}
