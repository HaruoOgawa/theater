precision highp float;
uniform sampler2D backbuffer;
uniform vec2 resolution;
uniform vec2 mouse;
uniform float time;

//メインオブジェクトの距離関数
float mainDist(vec3 p){
    p.z-=time;
    p = mod(p,0.8)-0.4;//modを使って複製
    return length(p)-0.2;//半径0.2の球を表示
}

void main(){
    //UV座標、正規化した座標の定義
    vec2 uv = gl_FragCoord.xy/resolution;
    vec2 r=resolution,p=(gl_FragCoord.xy*2.-r)/min(r.y,r.x);

    vec3 ro = vec3(0.0);//ray originの定義
    vec3 rd =  normalize(vec3(p, -1.0));//ray directionの定義

    //レイマーチングをする際に必要な変数の定義
    float d,t=0.;//dはdist tはtotal distanceの略

    //レイマーチングを行う
    for(int i = 0;i<76;i++){
        d = mainDist(ro+rd*t);//メインオブジェクトの距離関数を計算
          if(d<0.001) break;
          t+=d;//レイを進める
    }
	  vec3 col =vec3(exp(-0.4*t));

    gl_FragColor=vec4(col,1);//色を出力

}
