R"(

#version 330

in vec2 uv;
in vec3 WorldNormal;
in vec3 WorldVertexPos;

uniform vec3 _WorldCameraPos;
uniform vec3 _WorldCameraCenter;
uniform float _time;
uniform vec2 _resolution;
uniform float _RenderingTarget;

///////////////////////////////////////////////////////////////////
/////////// I studied from the following site by iq.    ///////////
/////////// https://www.youtube.com/watch?v=BFld4EBO2RE ///////////
/////////// https://www.shadertoy.com/view/XsXfRH       ///////////
/////////// https://iquilezles.org/articles/fbm/        ///////////
///////////////////////////////////////////////////////////////////

// Base Func ///////////////////////////////////////////////////
float hash(vec3 p)
{
    p=50.0*fract( p*0.3183099 + vec3(0.71,0.113,0.419));
    return -1.0+2.0*fract( p.x*p.y*p.z*(p.x+p.y+p.z) );
}

vec4 noised(in vec3 x)
{
    vec3 p=floor(x);
    vec3 w=fract(x);
    
    vec3 u=w*w*w*(w*(w*6.0-15.0)+10.0);
    vec3 du=30.0*w*w*(w*(w-2.0)+1.0);
    
    float a = hash( p+vec3(0,0,0) );
    float b = hash( p+vec3(1,0,0) );
    float c = hash( p+vec3(0,1,0) );
    float d = hash( p+vec3(1,1,0) );
    float e = hash( p+vec3(0,0,1) );
    float f = hash( p+vec3(1,0,1) );
    float g = hash( p+vec3(0,1,1) );
    float h = hash( p+vec3(1,1,1) );
    
    float k0 = a;
    float k1 = b-a;
    float k2 = c-a;
    float k3 = e-a;
    float k4 = a-b-c+d;
    float k5 = a-c-e+g;
    float k6 = a-b-e+f;
    float k7 =-a+b+c-d+e-f-g+h;
    
    return vec4( -1.0+2.0*(k0 + k1*u.x + k2*u.y + k3*u.z + k4*u.x*u.y + k5*u.y*u.z + k6*u.z*u.x + k7*u.x*u.y*u.z),
                 2.0* du * vec3( k1 + k4*u.y + k6*u.z + k7*u.y*u.z,
                                 k2 + k5*u.z + k4*u.x + k7*u.z*u.x,
                                 k3 + k6*u.x + k5*u.y + k7*u.x*u.y ) );
}

float plane(vec3 p,vec4 n)
{
    return dot(p,n.xyz)*n.w;
}

// Moutain Func ////////////////////////////////////////////
const mat3 m3  = mat3( 0.00,  0.80,  0.60,
                      -0.80,  0.36, -0.48,
                      -0.60, -0.48,  0.64 );
const mat3 m3i = mat3( 0.00, -0.80, -0.60,
                       0.80,  0.36, -0.48,
                       0.60, -0.48,  0.64 );

vec4 fbm(in vec3 x,int octaves)
{
    float f=1.9;
    float s=0.55;
    float a=0.0;
    float b=0.5;
    vec3 d=vec3(0.0);
    mat3 m=mat3(
        1.0,0.0,0.0,
        0.0,1.0,0.0,
        0.0,0.0,1.0
    );
    
    for(int i=0;i<octaves;i++)
    {
        vec4 n=noised(x);
        a+=b*n.x;
        d+=b*m*n.yzw;
        b*=s;
        x=f*m3*x;
        m=f*m3i*m;
    }
    
    return vec4(a,d);
}

float MountainMap(vec3 p)
{
   // p.z-=iTime*1000.0;
    float d = plane(p,vec4(0.0,1.0,0.0,2.0));
    float h = fbm(p/2000.0+vec3(vec2(1.0,-2.0),0.0),9).x;
    h=200.0*h+100.0;
    //h=2000.0*h+600.0;
    return d-h;
}

vec3 renderMountain(vec3 ro,vec3 rd,float dmax)
{
    float d=1.0,t=0.0,i=0.0;
    for(;++i<64.0 && (d>dmax );)
    {
        d=MountainMap(ro+rd*t);
        t+=d*0.175;
    }
    return vec3(d,t,dmax);
}

vec3 gnMountain(vec3 p,float dmax)
{
    vec2 e=vec2(dmax,0.0);
    return normalize(vec3(
        MountainMap(p+e.xyy)-MountainMap(p-e.xyy),
        MountainMap(p+e.yxy)-MountainMap(p-e.yxy),
        MountainMap(p+e.yyx)-MountainMap(p-e.yyx)
    ));
}

vec4 LightingMountain(vec3 ro,vec3 rd,float t,float dmax)
{
    vec4 col=vec4(1.0);

    vec3 n=gnMountain(ro+rd*t,0.75);
    float r=1.0;
    vec3 l = normalize(vec3(
        r *sin(1.3)*cos(2.36),
        r *cos(1.3),
        r *sin(1.3)*sin(2.36)
    ));
        
    float diff=dot(n,l);
    diff=max(diff,0.0);
    col.rgb*=diff;
    col.rgb*=vec3(211.0 / 255.0, 149.0 / 255.0, 107.0 / 255.0);
    //col.rgb=n;
        
    // fog
    //vec3 ramda = exp2(-0.000125*t*vec3(1.0,2.0,4.0));
    //col.rgb=mix(vec3(0.5),col.rgb,ramda);
        
    return col;
}

void main()
{
    //vec2 st = (gl_FragCoord.xy*2.0-_resolution.xy)/min(_resolution.x,_resolution.y);
    vec2 st=uv*2.0-1.0;
    st.x*=(_resolution.x/_resolution.y);
    
    vec4 col = vec4(vec3(0.0),1.0);
    vec3 ta=vec3(0.0,100.0,0.0),ro=vec3(0.0,200.0,200.0);
    vec3 cdir=normalize(ta-ro);
    vec3 cside=normalize(cross(cdir,vec3(0.0,-1.0,0.0)));
    vec3 cup=normalize(cross(cdir,cside));
    vec3 rd=normalize(st.x*cside+st.y*cup+1.0*cdir);
    float nowobj = 2.0,d=1.0,t=0.0;
    
    // sky color
    //col.rgb=vec3(0.42, 0.62, 1.1) - rd.y*0.4;
    
    // modeling of Mountain
    vec3 mountain = renderMountain(ro,rd,0.75);
    if(mountain.x<d)
    {
        d=mountain.x;
        t=mountain.y;
        nowobj=1.0;
    }
    
    // Lighting
    if(nowobj == 1.0) // moutain
    {
        col=LightingMountain(ro,rd,t,mountain.z);
    }

    gl_FragColor = col;
}

)"