#version 430

precision mediump float;

uniform vec2 _resolution;
uniform float _RenderingTarget;
uniform float _time;

uniform vec3 _camerePosition;

in vec2 uv;

#define rot(a) mat2(cos(a),-sin(a),sin(a),cos(a))

float rand(vec2 st)
{
    return fract(sin(dot(st, vec2(12.9898, 78.233))) * 43758.5453);
}

float Cube(vec3 p,vec3 s){
  p=abs(p);
  return length(max(p-s,0.0));
}

float map(vec3 p){
      float mt=floor(_time*0.25)+pow(fract(_time*0.25),.15);
  float styleVal=6.5;
  float trs_val=step(fract(_time),0.5);
 
   p.xy*=rot(mt);
  p.xz*=rot(mt);
  p.yz*=rot(mt);



  vec3 pos1=p;
  float d0=1000.0;
  for(int i=0;i<5;i++){
    pos1=abs(pos1)-0.25;
    if(pos1.x<pos1.y)pos1.xy=pos1.yx;
    if(pos1.x<pos1.z)pos1.xz=pos1.zx;
    if(pos1.y<pos1.z)pos1.yz=pos1.zy;
    // move
    float off=0.65+styleVal*0.1+0.1;
    pos1.x+=rand(vec2(i,0.156))*off-off*0.5;
    pos1.y+=rand(vec2(i,0.491))*off-off*0.5;
    pos1.z+=rand(vec2(i,0.791))*off-off*0.5;
    //scale
    float rs=1.25+styleVal*0.1+0.15;
    pos1*=rand(vec2(i,94.0))*rs+0.5;
    
  // rotate
  float rv=3.14159265*0.75*(
   trs_val
  );
  //pos1.xy*=rot(mt);
  pos1.yz*=rot(mt);
  pos1.yz*=rot(mt);
  //pos1.xz*=rot(mt);

    d0=min(d0,Cube(pos1,vec3(0.25,0.25,0.25)));

  }

  /*
  pos1.xy*=rot(rv*rand(vec2(0.791,i)));
  pos1.yz*=rot(rv*rand(vec2(0.913,i)));
  pos1.xz*=rot(rv*rand(vec2(0.123,i)));
  */

  return d0;
 
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
  vec3 col=vec3(1.0);

  //vec2 st=(gl_FragCoord.xy*2.0-_resolution.xy)/min(_resolution.x,_resolution.y);
  vec2 st=uv*2.0-1.0;
  st.x*=(_resolution.x/_resolution.y);
  //col=vec3(st,0.0);

  //camera
  //vec3 ro=vec3(0.0,0.0,1.0);
  vec3 ro=_camerePosition;
  vec3 ta=vec3(0.0);
  vec3 cd=normalize(ta-ro);
  vec3 cs=normalize(cross(cd,vec3(0.0,1.0,0.0)));
  vec3 cu=normalize(cross(cd,cs));
  float dep=1.0;
  vec3 rd=normalize(vec3(cs*st.x+cu*st.y+cd*dep));

  float d,t,acc=0.0;
  int march=0;
  int MAX_MARCH=128;
  for(int i=0;i<128;i++){
    d=map(ro+rd*t);
    march=i;
    if(d<0.001)break;
    t+=d;
    acc+=exp(-3.0*d);
    march=MAX_MARCH-1;
  }

  float glow=0.0;
  const float s = 0.05;
  vec3 n0=gn(ro+rd*t);
  vec3 n1=gn(ro+rd*t+vec3(sign(n0.x)*s,0.0,0.0));
  vec3 n2=gn(ro+rd*t+vec3(0.0,sign(n0.y)*s,0.0));

  glow=max(0.0,dot(n0,rd));

  float emw=0.8;
  if(dot(n0, n1)<emw || dot(n0, n2)<emw) {
           glow += 4.5;
       }

  glow *= min(1.0,
    4.0-(4.0*float(march) / float(MAX_MARCH-1))
  );

  if(_RenderingTarget==1.0){
    col=vec3(1.0)*acc*0.05;
    //col=vec3(1.0)*acc*0.1*glow;

  }else if(_RenderingTarget==2.0){
    col=clamp(1.0-vec3(1.0)*exp(-.0075*t),0.0,1.0);
  }

  //col=vec3(1.0);

  gl_FragColor=vec4(col,1.0);
}
