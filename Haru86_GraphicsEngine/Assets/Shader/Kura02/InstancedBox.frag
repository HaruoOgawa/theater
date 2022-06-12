#version 430

uniform vec3 _CameraPosition;
uniform vec3 _lightDir;
uniform float _Roughness;
uniform float _FresnelReflectance;

in vec3 worldPosition;
in vec3 worldNormal;
in float id;
in vec3 randColor;
in vec2 uv;

#define rot(a) mat2(cos(a),-sin(a),sin(a),cos(a))

float rand(vec2 seeds){
  return fract(sin(dot(seeds, vec2(12.9898,78.233))) * 43758.5453);
}

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
	vec3 col=vec3(0.0);

	//diffuse
	/*vec3 L=normalize(_lightDir);
	float diff=max(0.0,dot(worldNormal,L));
	col.rgb*=diff;
	col+=0.01;*/
    

    //PBR///////////////////
     
    // ワールド空間上のライト位置と法線との内積を計算
    vec3 lightDirectionNormal = normalize(_lightDir);
    float NdotL = clamp(dot(worldNormal, lightDirectionNormal),0.0,1.0);

    // ワールド空間上の視点（カメラ）位置と法線との内積を計算
    vec3 viewDirectionNormal = normalize((_CameraPosition - worldPosition).xyz);
    float NdotV = clamp(dot(worldNormal, viewDirectionNormal),0.0,1.0);

    // ライトと視点ベクトルのハーフベクトルを計算
    vec3 halfVector = normalize(lightDirectionNormal + viewDirectionNormal);

    // D_GGXの項
    float D = D_GGX(halfVector,worldNormal);

    // Fの項
    float F = Flesnel(viewDirectionNormal, halfVector);

    // Gの項
    float G = G_CookTorrance(lightDirectionNormal, viewDirectionNormal, halfVector,worldNormal);

    // スペキュラおよびディフューズを計算
    float specularReflection = (D * F * G) / (4.0 * NdotV * NdotL + 0.000001);
  
    vec3 pbrCol=vec3(1.0);

    col=col+pbrCol*specularReflection;
    
    col+=randColor;

    vec2 st=uv*2.0-1.0;
    col=col*step(abs(st.x),0.9)*step(abs(st.y),0.9);

    //col=vec3(uv,0.0);
	//gl_FragColor=vec4(col,0.5);
	gl_FragColor=vec4(col,1.0);
}