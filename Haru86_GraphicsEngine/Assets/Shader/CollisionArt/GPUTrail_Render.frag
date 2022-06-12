#version 430

struct g2f{
	vec4 vertex ;
	vec2 uv ;
	float node_life;
	float trail_ID ;
	vec4 trail_color;
	vec3 normal;
    vec3 worldPosition;
};

in float g2f_flag;
in g2f g2f_o;

uniform float _time;
uniform float _initNodeLife;
uniform sampler2D _trailTex;
uniform vec3 _CameraPosition;

uniform vec3 _lightDir;
uniform float _Roughness;
uniform float _FresnelReflectance;

#define LightDir vec3(-0.613121, -0.1916, 0.766402)

// D（GGX）の項
float D_GGX(vec3 H, vec3 N) {
    float NdotH = clamp(dot(H, N),0.0,1.0);
    float roughness = clamp(1.0-_Roughness,0.0,1.0);
    float alpha = roughness * roughness;
    float alpha2 = alpha * alpha;
    float t = ((NdotH * NdotH) * (alpha2 - 1.0) + 1.0);
    float PI = 3.1415926535897;
    return alpha2 / (PI * t * t);
}

// フレネルの項
float Flesnel(vec3 V, vec3 H) {
    float VdotH = clamp(dot(V, H),0.0,1.0);
    float F0 = clamp(_FresnelReflectance,0.0,1.0);
    float F = pow(1.0 - VdotH, 5.0);
    F *= (1.0 - F0);
    F += F0;
    return F;
}

// G - 幾何減衰の項（クック トランスモデル）
float G_CookTorrance(vec3 L, vec3 V, vec3 H, vec3 N) {
    float NdotH = clamp(dot(N, H),0.0,1.0);
    float NdotL = clamp(dot(N, L),0.0,1.0);
    float NdotV = clamp(dot(N, V),0.0,1.0);
    float VdotH = clamp(dot(V, H),0.0,1.0);

    float NH2 = 2.0 * NdotH;
    float g1 = (NH2 * NdotV) / VdotH;
    float g2 = (NH2 * NdotL) / VdotH;
    float G = min(1.0, min(g1, g2));
    return G;
}


void main(){
	vec4 col=vec4(1.0);
	
	//diffuse
	vec3 L=normalize(_lightDir);
	float diff=max(0.0,dot(g2f_o.normal,L));
	col.rgb*=diff;
	col+=0.01;

    //PBR///////////////////
     
    // ワールド空間上のライト位置と法線との内積を計算
    vec3 lightDirectionNormal = normalize(_lightDir);
    float NdotL = clamp(dot(g2f_o.normal, lightDirectionNormal),0.0,1.0);

    // ワールド空間上の視点（カメラ）位置と法線との内積を計算
    vec3 viewDirectionNormal = normalize((_CameraPosition - g2f_o.worldPosition).xyz);
    float NdotV = clamp(dot(g2f_o.normal, viewDirectionNormal),0.0,1.0);

    // ライトと視点ベクトルのハーフベクトルを計算
    vec3 halfVector = normalize(lightDirectionNormal + viewDirectionNormal);

    // D_GGXの項
    float D = D_GGX(halfVector,g2f_o.normal);

    // Fの項
    float F = Flesnel(viewDirectionNormal, halfVector);

    // Gの項
    float G = G_CookTorrance(lightDirectionNormal, viewDirectionNormal, halfVector,g2f_o.normal);

    // スペキュラおよびディフューズを計算
    float specularReflection = (D * F * G) / (4.0 * NdotV * NdotL + 0.000001);
  
    vec3 pbrCol=vec3(1.0);

    col.rgb=col.rgb+pbrCol*specularReflection;

    ///////////////////////////////////////

	//col.rgb=g2f_o.trail_color.rgb;
	//col.a=g2f_o.trail_color.a;

	//vec2 st=mod(g2f_o.uv,5.0);
	//st.x*=0.2;
	vec2 st=g2f_o.uv*2.0-1.0;
    //col.rgb=col.rgb*step(abs(st.x),0.9)*step(abs(st.y),0.9);

	//col.rgb=vec3(g2f_o.uv,0.0);

	gl_FragColor=col;
}
