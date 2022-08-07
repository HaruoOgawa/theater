R"(

#version 410

layout(triangles) in;
layout(triangle_strip,max_vertices=10) out;

uniform mat4 MVPMatrix;
uniform mat4 MMatrix;
uniform mat4 VMatrix;
uniform mat4 PMatrix;
uniform float _time;
uniform float _deltaTime;
uniform vec3 _CameraPos;

in float t2g_pid[];
in vec3 t2g_normal[];
in vec4 t2g_billinfo[];
in vec3 t2g_WorldVertexPos[];

out float g2f_pid;
out vec3 g2f_normal;
out vec4 g2f_billinfo;
out vec3 g2f_WorldVertexPos;

void Createvertex(vec3 offset)
{
	gl_Position = MVPMatrix * (gl_in[0].gl_Position + vec4(offset,0.0));
	g2f_pid=t2g_pid[0];
	g2f_WorldVertexPos = (MMatrix* gl_in[0].gl_Position).xyz;
	g2f_normal = t2g_normal[0];
	g2f_billinfo = t2g_billinfo[0];

	EmitVertex();

	gl_Position = MVPMatrix * (gl_in[1].gl_Position + vec4(offset,0.0));
	g2f_pid=t2g_pid[1];
	g2f_WorldVertexPos = (MMatrix* gl_in[1].gl_Position).xyz;
	g2f_normal = t2g_normal[1];
	g2f_billinfo = t2g_billinfo[1];

	EmitVertex();

	gl_Position = MVPMatrix * (gl_in[2].gl_Position + vec4(offset,0.0));
	g2f_pid=t2g_pid[2];
	g2f_WorldVertexPos = (MMatrix* gl_in[2].gl_Position).xyz;
	g2f_normal = t2g_normal[2];
	g2f_billinfo = t2g_billinfo[2];

	EmitVertex();

	EndPrimitive();
}

void main()
{
	// 基礎
	Createvertex(vec3(0.0));

	/*// ビルの凹凸
	{
		vec3 localPos = vertex;
		float localRadius = BillRadius * AdjustVal;
		int numOfExtrude = int(floor( rand(vec2(id+6.411))*2.99 ));

		for(int n = 0; n<numOfExtrude; n++)
		{
			// どの軸に押しだすのかを決める
			// 符号で以下のように定義する。ただし横軸がX、縦軸がZとする
			// → => (x,z)=(+,+)
			// ← => (x,z)=(+,-)
			// ↑ => (x,z)=(-,+)
			// ↓ => (x,z)=(-,-)
			
			vec3 noise0 = hash(vec3(id+id+11.291,id-9.0,id+17.0));
			vec2 ev = vec2( sign(noise0.x)*1.0, sign(noise0.y)*1.0 ) ; // => ExtrudeVector

			// 押し出す範囲内に収まっているかを確認する
			// noise0は符号しか使っていなくてもったいないので符号だけ乱数で計算して使い回す
			float randSign = sign( rand( vec2(noise0.x+noise0.y+noise0.z) )*2.0-1.0 );

			float ExtrudeStart = localRadius * min(randSign*noise0.z,randSign*noise0.y);
			float ExtrudeEnd = localRadius * max(randSign*noise0.z,randSign*noise0.y);
			
			// 押し出す
			float ExtrudeVolume = noise0.x*2.0;
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
					pos = ExtrudeMatrix * pos;
				}
			}
			else // Z軸方向に押しだす
			{
				// チェック
				if( localPos.x>=ExtrudeStart && localPos.x<=ExtrudeEnd)
				{
					// 押し出す
					ExtrudeMatrix[3][2] = ExtrudeVolume;
					pos = ExtrudeMatrix * pos;
				}
			}
		}
	}*/

}

)"