R"(

struct SoundOutput{
    vec2 s;
    vec2 outs;
    float samplerOffset;
    float samplerWidth;
};

#define PI 3.14159265
 #define PI2 3.14159265*2.0

// サンプラーサウンドのデバッグ(ただの試し弾き)
 #define dso(sound,useMet) debugSound(sound,ds,isd,t,beat,beatf,useMet)
 // サンプラー波形のデバッグ
 #define DebugSampler(sampler,o,w) dsw=sampler; samplerOffset=o;samplerWidth=w

 #define BPM 175.0
 
 #define saturate(i) clamp(i,-1.0,1.0)
 #define lofi(i,j) floor((i)/j)*j
 #define b2t(i) ((i)/BPM*60.0)
 
 #define calscale(r) 440.0*pow(2.0,(r)/12.0)
 #define scale(octave,note) calscale(octave*12.0+note)

 #define A 0.0
 #define B 2.0
 #define C 3.0
 #define D 5.0
 #define E 7.0
 #define F 8.0
 #define G 10.0
 #define rest -10000.0
 
 float rand(float time)
{
    return fract(sin(time*1e3)*1e6)-0.5;
}

// ---------------------------
// 基礎的な音

float saw(float phase){
    return 2.0*fract(phase)-1.0;
}

float square(float phase){
    return fract(phase) <0.5 ? -1.0 : 1.0;
}

float triangle(float phase){
    return 1.0-4.0*abs(fract(phase)-0.5);
}

float sine(float phase){
    return sin(PI2*phase);
}

float synth0(float phase){
    float vib=0.2*sine(phase);
   // vib=0.0;
    return triangle(phase+vib);
}

float synth1(float phase){
    float fm=0.1*sine(phase*3.0);
    return saw(phase+fm)*sine(phase+fm);
}

// シンセ和音の生成(サンプラーだけを返す)
float csaw(float phase,float s0,float s1,float s2,float s3){ // chords saw
    float o=0.0;
    o+=saw(phase*s0);
    o+=saw(phase*s1);
    o+=saw(phase*s2);
    o+=saw(phase*s3);
    
    return o*0.25;
}

float csquare(float phase,float s0,float s1,float s2,float s3){ // chords square
    float o=0.0;
    o+=square(phase*s0);
    o+=square(phase*s1);
    o+=square(phase*s2);
    o+=square(phase*s3);
    
    return o*0.25;
}

float ctriangle(float phase,float s0,float s1,float s2,float s3){ // chords triangle
    float o=0.0;
    o+=synth1(phase*s0);
    o+=synth1(phase*s1);
    o+=synth1(phase*s2);
    o+=synth1(phase*s3);
    
    return o;
}

float csine(float phase,float s0,float s1,float s2,float s3){ // chords sine
    float o=0.0;
    o+=sine(phase*s0);
    o+=sine(phase*s1);
    o+=sine(phase*s2);
    o+=sine(phase*s3);
    
    return o*0.25;
}
// ------------------------------

// Instruments(楽器)
// _phase => 時間???

// 楽器の定義ではループは設定しないようにしている
// DAWでのサンプラーの波形をそのまま表現するようにしている。
// DAWで波形をみて楽器を作るようにする!!!!!!!!!!!!!

float kick(float phase){
    if(phase<0.0)return 0.0;
    return sin(phase * 300.0 - exp(-phase * 70.0)*80.0) * exp(-phase * 5.0);
}

// Kick => サイン波のだんだんと位相が広がっていく感じ
float kick2(float phase){
    if(phase < 0.0){return 0.0;}
    return sin(phase *300.0 - exp(-phase *70.0)*80.0)*exp(-phase *4.0);
}

float snare(float phase){
    if(phase<0.0)return 0.0;
    
    return saturate(
        (
            rand(phase/0.034) +
            sin(phase*2500.0 -exp(-phase * 400.0) * 30.0)
        ) *  2.0 *exp(-phase *23.0)
    );
}

// snare => ホワイトノイズっぽい音をサイン波で制御する感じ
float snare2(float phase){
    if(phase<0.0)return 0.0;
    return (
        rand(lofi(phase,6E-5)/2.06)*0.5+
        sin(phase * 2000.0  - exp(-phase*800.0) * 20.0)
    )*exp(-phase*31.0);
}

// hihat =>普通に短く区切ったホワイトノイズ
float hihat(float _seed,float _dec){
    return rand(_seed)*exp(-_dec);
}

//
float sharpSaw(float _phase){
    return mod(_phase,1.0) * 2.0 -1.0;
}

float bassDrum(float time)
{
    return sin(time * scale(0.0,9.0)) * max(0.0, 1.0 - fract(time) * 8.0);
}

// ダイアトニックコードを定義 -------------------------------------
// ひとまずCメジャースケールで作ってみる

#define sampler csine

#define CM7(phase,o) sampler(phase,scale(o,C),scale(o,E),scale(o,G),scale(o+1.0,B))
#define Dm7(phase,o) sampler(phase,scale(o,D),scale(o,F),scale(o+1.0,A),scale(o+1.0,C))
#define Em7(phase,o) sampler(phase,scale(o,E),scale(o,G),scale(o+1.0,B),scale(o+1.0,D))
#define FM7(phase,o) sampler(phase,scale(o,F),scale(o+1.0,A),scale(o+1.0,C),scale(o+1.0,E))
#define G7(phase,o)  sampler(phase,scale(o,G),scale(o+1.0,B),scale(o+1.0,D),scale(o+1.0,F))
#define Am7(phase,o) sampler(phase,scale(o+1.0,A),scale(o+1.0,C),scale(o+1.0,E),scale(o+1.0,G))
#define Bm7(phase,o) sampler(phase,scale(o+1.0,B),scale(o+1.0,D),scale(o+1.0,F),scale(o+2.0,A))

// bar(小節) -------------------------------------------------
// 1小節(合計4泊)ごとに関数でまとめる
float bar0(float beat,float masterOffset,float loopBeatNum,float octave){
    float ret=0.0;
    ret+=CM7(b2t(beat),octave)*pow(4.0,-4.0*mod(beat - 0.0 - masterOffset,loopBeatNum))*0.2;
    ret+=CM7(b2t(beat),octave)*pow(4.0,-16.0*mod(beat - 1.0 - masterOffset,loopBeatNum))*0.2;
    ret+=Dm7(b2t(beat),octave)*pow(4.0,-16.0*mod(beat - 1.25 - masterOffset,loopBeatNum))*0.2;
    ret*=( (pow(4.0,-16.0*mod(beat - 1.5 - masterOffset,loopBeatNum))<0.5)? 1.0 : 0.0 ) ;
    ret+=FM7(b2t(beat),octave)*pow(4.0,-4.0*mod(beat - 1.75 - masterOffset,loopBeatNum))*0.2;
    ret+=FM7(b2t(beat),octave)*pow(4.0,-16.0*mod(beat - 2.75 - masterOffset,loopBeatNum))*0.2;
    ret+=Em7(b2t(beat),octave)*pow(4.0,-16.0*mod(beat - 3.0 - masterOffset,loopBeatNum))*0.2;
    ret+=Dm7(b2t(beat),octave)*pow(4.0,-16.0*mod(beat - 3.25 - masterOffset,loopBeatNum))*0.2;
    ret+=CM7(b2t(beat),octave)*pow(4.0,-8.0*mod(beat - 3.5 - masterOffset,loopBeatNum))*0.2;
    
    return ret;
}


float bar1(float beat,float masterOffset,float loopBeatNum,float octave){
    float ret=0.0;
    ret+=CM7(b2t(beat),octave)*pow(4.0,-4.0*mod(beat - 0.0 - masterOffset,loopBeatNum))*0.2;
    ret+=CM7(b2t(beat),octave)*pow(4.0,-16.0*mod(beat - 1.0 - masterOffset,loopBeatNum))*0.2;
    ret+=Dm7(b2t(beat),octave)*pow(4.0,-16.0*mod(beat - 1.25 - masterOffset,loopBeatNum))*0.2;
    ret*=( (pow(4.0,-16.0*mod(beat - 1.5 - masterOffset,loopBeatNum))<0.5)? 1.0 : 0.0 ) ;
    ret+=G7(b2t(beat),octave)*pow(4.0,-4.0*mod(beat - 1.75 - masterOffset,loopBeatNum))*0.2;
    ret+=FM7(b2t(beat),octave)*pow(4.0,-16.0*mod(beat - 2.75 - masterOffset,loopBeatNum))*0.2;
    ret+=Em7(b2t(beat),octave)*pow(4.0,-16.0*mod(beat - 3.0 - masterOffset,loopBeatNum))*0.2;
    ret+=Dm7(b2t(beat),octave)*pow(4.0,-16.0*mod(beat - 3.25 - masterOffset,loopBeatNum))*0.2;
    ret+=CM7(b2t(beat),octave)*pow(4.0,-8.0*mod(beat - 3.5 - masterOffset,loopBeatNum))*0.2;
    
    return ret;
}

float bar2(float beat,float masterOffset,float loopBeatNum,float octave){
    float ret=0.0;
    ret+=CM7(b2t(beat),octave)*pow(4.0,-4.0*mod(beat - 0.0 - masterOffset,loopBeatNum))*0.2;
    ret+=CM7(b2t(beat),octave)*pow(4.0,-16.0*mod(beat - 1.0 - masterOffset,loopBeatNum))*0.2;
    ret+=Dm7(b2t(beat),octave)*pow(4.0,-16.0*mod(beat - 1.25 - masterOffset,loopBeatNum))*0.2;
    ret*=( (pow(4.0,-16.0*mod(beat - 1.5 - masterOffset,loopBeatNum))<0.5)? 1.0 : 0.0 ) ;
    ret+=Em7(b2t(beat),octave)*pow(4.0,-4.0*mod(beat - 1.75 - masterOffset,loopBeatNum))*0.2;
    ret+=Dm7(b2t(beat),octave)*pow(4.0,-16.0*mod(beat - 2.75 - masterOffset,loopBeatNum))*0.2;
    ret+=Dm7(b2t(beat),octave)*pow(4.0,-16.0*mod(beat - 3.0 - masterOffset,loopBeatNum))*0.2;
    ret+=CM7(b2t(beat),octave)*pow(4.0,-16.0*mod(beat - 3.25 - masterOffset,loopBeatNum))*0.2;
    ret+=CM7(b2t(beat),octave)*pow(4.0,-8.0*mod(beat - 3.5 - masterOffset,loopBeatNum))*0.2;
    
    return ret;
}

float bar3(float beat,float masterOffset,float loopBeatNum,float octave){
    float ret=0.0;
    ret+=CM7(b2t(beat),octave)*pow(4.0,-4.0*mod(beat - 0.0 - masterOffset,loopBeatNum))*0.2;
    ret+=CM7(b2t(beat),octave)*pow(4.0,-16.0*mod(beat - 1.0 - masterOffset,loopBeatNum))*0.2;
    ret+=Dm7(b2t(beat),octave)*pow(4.0,-16.0*mod(beat - 1.25 - masterOffset,loopBeatNum))*0.2;
    ret*=( (pow(4.0,-16.0*mod(beat - 1.5 - masterOffset,loopBeatNum))<0.5)? 1.0 : 0.0 ) ;
    ret+=G7(b2t(beat),octave)*pow(4.0,-4.0*mod(beat - 1.75 - masterOffset,loopBeatNum))*0.2;
    ret+=FM7(b2t(beat),octave)*pow(4.0,-16.0*mod(beat - 2.75 - masterOffset,loopBeatNum))*0.2;
    ret+=Em7(b2t(beat),octave)*pow(4.0,-16.0*mod(beat - 3.0 - masterOffset,loopBeatNum))*0.2;
    ret+=Dm7(b2t(beat),octave)*pow(4.0,-16.0*mod(beat - 3.25 - masterOffset,loopBeatNum))*0.2;
    ret+=Dm7(b2t(beat),octave)*pow(4.0,-8.0*mod(beat - 3.5 - masterOffset,loopBeatNum))*0.2;
 
    return ret;
}


// -------------------------------

// サンプラーサウンドのデバッグ(ただの試し弾き)
void debugSound(float sound,inout float o,out bool isDebug,float t,float beat,float beatf,bool useMet){
    o=sound;
    if(useMet)o+=0.5*sine(t * (mod(beat,4.0) < 1.0 ? 2000.0 : 1000.0) ) * (beatf < 0.1 ? 1.0 : 0.0)*0.5;
    isDebug=true;
}

// _________________________

// pow(4.0,-1.0*x) => これだと4泊(全音符・1小節)で減衰 
   // pow(4.0,-2.0*x) => これだと2泊(二分音符)で減衰
   // pow(4.0,-4.0*x) => これだと1泊(四分音符)で減衰
   // エンベロープを一般化すると...
   // pow(4.0,-音符の種類*mod(beat-offset , 繰り返しのbeat間隔))
   // offset = その音が鳴るまでの小節内の合計拍
   // 合計拍、数えるの大変なので1小節(全部で4泊)で関数まとめるといいかも？

// 極力コメントを振る
SoundOutput DrawWave(float time){
    // 試し弾き用
    bool isd=false; // isDebugMode
    float ds=0.0;   // サンプラーサウンド
    float dsw=0.0;  // サンプラーの波形
    float samplerOffset=0.0;
    float samplerWidth=1.0;

    // 基本パラメーターの用意 -------------------------
    
    float t=time;
    float beat=t*(BPM/60.0)-8.0; // 8.0 => pi po po po pi po po po
    // BPMを使うと拍(メトロノームのやつ)で曲のスピードが考えられるから嬉しい
    
    float ret=0.0;
    float tenkai=floor(beat/4.0); // ???
    float sideChain=0.0; // ???
    
    float beati=floor(beat);
    float beatf=fract(beat);
    float beat32=mod(beat,32.0);
    
    float kickTime;
    float snareTime;
    
    // -------------------------------------------------
   
    // pow(4.0,-1.0*x) => これだと4泊(全音符・1小節)で減衰 
    // pow(4.0,-2.0*x) => これだと2泊(2分音符)で減衰
    // pow(4.0,-4.0*x) => これだと1泊(4部音符)で減衰
   
    // 『-------』 は、たぶんトラックごとに分けるやつやと思う
   
    // -------
    
    // pi po po po
    // なるへそ、ビートだとこんな感じで感覚的に音と時間を合わせやすいのかぁ
    if(beat < 0.0){
        ret+=0.5*sine(t * (mod(beat,4.0) < 1.0 ? 2000.0 : 1000.0) ) * (beatf < 0.1 ? 1.0 : 0.0);
    }
    
    // -------
    
    // weaker kick snare part
    if( (beat > 0.0 && beat <64.0) || (beat > 192.0 && beat <256.0) )
    {
        kickTime=b2t( mod(mod(beat,4.0),2.5) );
        snareTime=b2t(mod(beat-1.0,2.0));
        float beat64=mod(beat,64.0);
        
        ret+=0.7*kick2(kickTime);
        ret+=0.5*snare2(snareTime);
        
        sideChain=smoothstep(0.0,0.2,min(snareTime,snareTime));
        ret+=0.2*sideChain*hihat(t,b2t(mod(beat,0.5))*100.0);
        if(beat > 32.0){
            ret+=0.2*hihat(t*0.1,b2t(mod(beat,0.25))*1000.0);
        }
        float trans=3.0;
        float build = max(0.0,beat64-48.0);
        float ksk=pow(build*0.3,2.0);
        float vib=sin(t*(20.0+ksk))*(0.1+ksk*0.1);
        ret+=sideChain*sharpSaw(scale(-2.0,A+trans)*t+vib)*0.04;
        ret+=sideChain*sharpSaw(scale(-2.0,E+trans)*t+vib)*0.04;
        ret+=sideChain*sharpSaw(scale(-2.0,G+trans)*t+vib)*0.04;
        ret+=sideChain*sharpSaw(scale(-1.0,D+trans)*t+vib)*0.04;
        ret+=sideChain*sharpSaw(scale(0.0,B+trans)*t+vib)*0.04;
        
        ret+=sideChain*0.2*build/16.0*rand(lofi(t*6.24,0.0005*lofi(build/16.0,0.02)));
        
        if(beat64 > 62.5){
             ret=0.7*kick(b2t(beat64-62.5));
             ret+=0.5*snare(b2t(beat64-63.0));
             if(beat > 255.0){
                 ret=0.5*snare(b2t(beat-255.0 - lofi(beat-255.0,0.08)*0.8));
             }
        }
    }
    
    //debug(sharpSaw(b2t(mod(beat,4.0))),false);
    DebugSampler(kick(t),0.5,0.75);
    
    // -------
    
    // first half
    if ( 64.0 < beat && beat < 192.0 ){
  
    }
    
    // second half
    if ( 256.0 < beat && beat < 448.0 ){
    
    }
    
    // -------
    
    // 音の最終アウトプット
    if(isd){
        SoundOutput o;
        o.s=vec2(ds);
        o.outs=vec2(dsw);
        o.samplerOffset=samplerOffset;
        o.samplerWidth=samplerWidth;
        
        return o;
    }else{
        SoundOutput o;
        o.s=vec2(ret);
        o.outs=vec2(dsw);
        o.samplerOffset=samplerOffset;
        o.samplerWidth=samplerWidth;
    
        return o;
    }
}

vec2 mainSound(float time){
    vec2 s=DrawWave(time).s;
    return s;
}

///////////////////////////

uniform float _sampleRate;
uniform float _offset;

in vec2 uv;

void main(){
    gl_FragColor=vec4(1.0);
}

)"