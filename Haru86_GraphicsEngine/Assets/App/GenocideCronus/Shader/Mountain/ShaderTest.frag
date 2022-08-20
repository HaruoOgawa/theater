R"(

#version 330
uniform float _time;
uniform vec2 _resolution;
uniform float _RenderingTarget;
uniform vec3 _WorldCameraPos;

in vec2 uv;

///////////////////////////////////////////////////////////////////
/////////// I studied from the following site by iq.    ///////////
/////////// https://www.youtube.com/watch?v=BFld4EBO2RE ///////////
/////////// https://www.shadertoy.com/view/XsXfRH       ///////////
/////////// https://iquilezles.org/articles/fbm/        ///////////
///////////////////////////////////////////////////////////////////

// preprocessor
//#define _time iTime
//#define _resolution iResolution

// Main
void main()
{
    // Base Parm
    //vec2 st = (fragCoord.xy*2.0-_resolution.xy)/min(_resolution.x,_resolution.y);
    vec2 st=uv*2.0-1.0;
    //st.x*=(_resolution.x/_resolution.y);
    
	vec3 col=vec3(0.0);
	col.rgb=vec3(st,0.0);

	if(_RenderingTarget==2.0) // ZTest
	{
		gl_FragColor = vec4(vec3(0.0),1.0);
	}
	else
	{
	    gl_FragColor = vec4(col,1.0);
	}
}

)"