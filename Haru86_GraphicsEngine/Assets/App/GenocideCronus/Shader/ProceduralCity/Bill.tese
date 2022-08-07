R"(

#version 410

layout(triangles, equal_spacing, ccw) in;

in vec3 tesc2tese_normal[];
in vec4 tesc2tese_billinfo[];
in float tesc2tese_id[];

out float t2g_id;
out vec3 t2g_normal;
out vec4 t2g_billinfo;
out vec3 t2g_WorldVertexPos;

const float VerDelimiterNum = 10.0;
const float BillRadius = 5.0;
const float AdjustVal = 0.1;

// hash --> https://www.shadertoy.com/view/4dffRH
vec3 hash( vec3 p ) // replace this by something better. really. do
{
	p = vec3( dot(p,vec3(127.1,311.7, 74.7)),
			  dot(p,vec3(269.5,183.3,246.1)),
			  dot(p,vec3(113.5,271.9,124.6)));

	return -1.0 + 2.0*fract(sin(p)*43758.5453123);
}

 float rand(vec2 st)
{
    return fract(sin(dot(st, vec2(12.9898, 78.233))) * 43758.5453);
}

void main(){
    //
	vec4 v0=gl_in[0].gl_Position;
	vec4 v1=gl_in[1].gl_Position;
	vec4 v2=gl_in[2].gl_Position;
	
    //
    vec3 normal=normalize(
         tesc2tese_normal[0]*gl_TessCoord.x +
         tesc2tese_normal[1]*gl_TessCoord.y + 
         tesc2tese_normal[2]*gl_TessCoord.z 
    );
    t2g_normal=normal.xyz;

    //
	vec4 pos=vec4(v0.xyz*gl_TessCoord.x+v1.xyz*gl_TessCoord.y+v2.xyz*gl_TessCoord.z,1.0); 
    
    // ビルデータを整理
	bool IsWindow = (tesc2tese_billinfo[0].x == 1.0);
	bool IsVertical = (tesc2tese_billinfo[0].y == 1.0);
	float DilimiterOrder = tesc2tese_billinfo[0].z;
	bool IsXAxis = (tesc2tese_billinfo[0].w == 1.0);
    
    // ビルの凹凸
	{
		vec3 localPos = pos.xyz;
		float localRadius = BillRadius * AdjustVal;
		int numOfExtrude = int(floor( rand(vec2(tesc2tese_id[0]+6.411))*2.99 ));

		for(int n = 0; n<numOfExtrude; n++)
		{
			// どの軸に押しだすのかを決める
			// 符号で以下のように定義する。ただし横軸がX、縦軸がZとする
			// → => (x,z)=(+,+)
			// ← => (x,z)=(+,-)
			// ↑ => (x,z)=(-,+)
			// ↓ => (x,z)=(-,-)
			
			vec3 noise0 = hash(vec3(tesc2tese_id[0]+tesc2tese_id[0]+11.291,tesc2tese_id[0]-9.0,tesc2tese_id[0]+17.0));
			vec2 ev = vec2( sign(noise0.x)*1.0, sign(noise0.y)*1.0 ) ; // => ExtrudeVector

			// 押し出す範囲内に収まっているかを確認する
			// noise0は符号しか使っていなくてもったいないので符号だけ乱数で計算して使い回す
			float randSign = sign( rand( vec2(noise0.x+noise0.y+noise0.z) )*2.0-1.0 );

			float ExtrudeStart = localRadius * min(randSign*noise0.z,randSign*noise0.y);
			float ExtrudeEnd = localRadius * max(randSign*noise0.z,randSign*noise0.y);
			
			// 押し出す
			float ExtrudeVolume = (noise0.x*0.5+0.5) * ev.y * 0.2;
			//float ExtrudeVolume = noise0.x*0.5;
			mat4 ExtrudeMatrix = mat4(
				vec4(0.0,0.0,0.0,0.0),
				vec4(0.0,0.0,0.0,0.0),
				vec4(0.0,0.0,0.0,0.0),
				vec4(0.0,0.0,0.0,1.0)
			); 

			if(ev.x == 1.0) // X軸方向に押しだす
			{
				// チェック
				if( localPos.z>=ExtrudeStart && localPos.z<=ExtrudeEnd)
				{
					// 押し出す
					ExtrudeMatrix[3][0] = ExtrudeVolume;
					//pos = ExtrudeMatrix * pos;
					pos.x+=ExtrudeVolume;
				}
			}
			else // Z軸方向に押しだす
			{
				// チェック
				if( localPos.x>=ExtrudeStart && localPos.x<=ExtrudeEnd)
				{
					// 押し出す
					ExtrudeMatrix[3][2] = ExtrudeVolume;
					//pos = ExtrudeMatrix * pos;
					pos.z+=ExtrudeVolume;
				}
			}
		}
	}

	// Debug
    /*if(IsXAxis)
    {
        if(localpos.z>=0.0 && localpos.z<0.2)
        {
            pos.x+=0.2;
        }
    }*/

    // Output
    gl_Position=pos;
	t2g_id=tesc2tese_id[0];
    t2g_billinfo=tesc2tese_billinfo[0];
    t2g_WorldVertexPos=pos.xyz;
}

)"