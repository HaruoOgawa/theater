#version 430

struct g2f{
	vec4 vertex ;
	vec2 uv ;
	float node_life;
	float trail_ID ;
	vec4 trail_color;
};

in float g2f_flag;
in g2f g2f_o;

uniform float _time;
uniform float _initNodeLife;
uniform sampler2D _trailTex;

#define LightDir vec3(-0.613121, -0.1916, 0.766402)

void main(){
	vec4 col=vec4(1.0);
	col.rgb=vec3(0.0);
	col.rgb=vec3(mod(g2f_o.uv,1.0),0.0);
	vec2 st=mod(g2f_o.uv,5.0);
	st.x*=0.2;
	col=texture(_trailTex,st);

	if(col.a<0.5){
		discard;
	}
	float node_life=1.0-clamp(_time*0.001-g2f_o.node_life,0.0,_initNodeLife)/_initNodeLife;
    col.a=node_life*1.0; 
	
	gl_FragColor=col;
}
