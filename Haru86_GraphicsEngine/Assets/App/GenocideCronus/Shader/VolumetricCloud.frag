R"(

#version 330

in vec2 uv;
in vec3 WorldNormal;
in vec3 WorldVertexPos;

uniform vec3 _WorldCameraPos;
uniform float _time;
uniform vec2 _resolution;
uniform float _RenderingTarget;

float rand(vec3 p) {
    return fract(sin(dot(p, vec3(12.345, 67.89, 412.12))) * 42123.45) * 2.0 - 1.0;
}

float perlin(vec3 p) {
    vec3 u = floor(p);
    vec3 v = fract(p);
    vec3 s = smoothstep(0.0, 1.0, v);
    
    float a = rand(u);
    float b = rand(u + vec3(1.0, 0.0, 0.0));
    float c = rand(u + vec3(0.0, 1.0, 0.0));
    float d = rand(u + vec3(1.0, 1.0, 0.0));
    float e = rand(u + vec3(0.0, 0.0, 1.0));
    float f = rand(u + vec3(1.0, 0.0, 1.0));
    float g = rand(u + vec3(0.0, 1.0, 1.0));
    float h = rand(u + vec3(1.0, 1.0, 1.0));
    
    return mix(mix(mix(a, b, s.x), mix(c, d, s.x), s.y),
               mix(mix(e, f, s.x), mix(g, h, s.x), s.y),
               s.z);
}

float fbm(vec3 p) {
    vec3 off = vec3(0.0, 0.5, 1.0) * _time;
    vec3 q = p - off;
    
    // fbm
    float f=0.0;
    float AM=0.5;
    float FA=2.0;
    for(int n=0;n<4;n++){
        f+=AM*perlin(q);
        q*=FA;
        AM*=0.5;
    }
    
    return clamp(f - p.y, 0.0, 1.0);
}

float map(vec3 p){
    float d0=fbm(p);
    return d0;
}

void main()
{
    //vec2 st = (gl_FragCoord.xy*2.0-_resolution.xy)/min(_resolution.x,_resolution.y);
    vec2 st=uv*2.0-1.0;
    st.x*=(_resolution.x/_resolution.y);
    
    vec4 col =vec4(0.4, 0.6, 1.0,0.0); 
    if(_RenderingTarget==2.0){
        col =vec4(vec3(1.0)*0.5,0.0); 
    }
    //vec4 col =vec4(0.0); 
    //vec4 col =vec4(0.85,0.85,0.85,0.0); 
    vec3 ta=vec3(0.0,0.0,0.0);
    //vec3 ro=vec3(0.0,1.0,1.0);
    vec3 ro=_WorldCameraPos;
    
    vec3 cdir=normalize(ta-ro);
    vec3 cside=normalize(cross(cdir,vec3(0.0,1.0,0.0)));
    vec3 cup=normalize(cross(cdir,cside));
    vec3 rd=normalize(st.x*cside+st.y*cup+1.0*cdir);
    
    float d=1.0,t=0.0,pi=0.0;
    for(;++pi<64.;){
        d=map(ro+rd*t);
        if(d>0.01){
            vec4 lcol=vec4( mix(vec3(0.0),vec3(1.0),d) ,d);
            lcol.a*=0.4;
            lcol.rgb*=lcol.a;
            
            col+=lcol*(1.0-col.a);
        }
        t+=max(0.05,0.02*d);
    }
    
    //col=clamp(col,0.0,1.0);
  
  if(_RenderingTarget==2.0){
    //col=vec4(0.0,0.0,0.0,1.0);
    //col=vec4( clamp(1.0-vec3(1.0)*exp(-.0075*t),0.0,1.0) , 1.0 );
  }

    gl_FragColor = col;
}

)"