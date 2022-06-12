#version 430

//BoidsData/////
struct BoidsData {
    vec3 position;
    vec3 velocity;
    vec3 viewDir;
};

layout(std430,binding=1) buffer destBufferBoidsData
{
	BoidsData boidsData[];
} outBufferBoidsData;

// outBufferBoidsData.boidsData[].position

//input/////
 struct input_data{
    vec3 nextInputPosition;
};

layout(std430,binding=2) buffer destBuffer_input_data
{
	input_data iData[];
} outBuffer_input_data;

// outBuffer_input_data.iData[].nextInputPosition

//node/////
struct node{
    vec3 node_position;
    float node_life;
    int renderFlag;
};

layout(std430,binding=3) buffer destBuffer_node
{
	node nodes[];
} outBuffer_node;

//trail///////
struct trail{
    int nextCalNodeIndex;
    int leftSideFirst;
    int rightSideFirst;
    int rightSideSecond;
    vec4 trail_color;
};

layout(std430,binding=4) buffer destBuffer_trail
{
	trail trails[];
} outBuffer_trail;

////////////////////////

layout(location=0)in vec3 vertex;
layout(location=1)in vec3 normal;
layout(location=2)in vec2 texcoord;

uniform float _time;
uniform float _deltaTime;
uniform float _initNodeLife;
uniform float _nodeSegment;
uniform float _TrailWidth;
uniform int _boids_count;

out float v2g_flag;
//out vec3 v2g_pos;

struct v2g{
    vec3 node_pos ;
    vec3 node_nextPos;
    vec3 node_dir;
    vec3 node_nextDir;
    float node_life;
    float trail_ID ;
    int nowIndexInNodes;
    vec4 trail_color;
};
out v2g v2g_o;


#define PI 3.14159265
#define rot(a) mat2(cos(a),sin(a),-sin(a),cos(a))

float rand(vec2 seeds){
  return fract(sin(dot(seeds, vec2(12.9898,78.233))) * 43758.5453);
}

//outBufferBoidsData.boidsData
// outBuffer_input_data.iData
//outBuffer_node.nodes
//outBuffer_trail.trails

//////////////////////////////

int CalCorrectIndex(int trailIndex,int calIndex){
    //trailIndex*_nodeSegment+_nodeSegment‚ð-1‚µ‚½‚çŽ¡‚Á‚½
    int index=int(min(trailIndex*_nodeSegment+_nodeSegment-1,max(trailIndex*_nodeSegment,calIndex)));
    return index;
}

void main(){
	
	int id=gl_InstanceID;
	float flag=1.0;
	if(id>_boids_count/2+1){
		flag=0.0;
	}else{
		flag=1.0;
	}
	v2g_flag=flag;

	//GPU Trail/////////////////////////////////////////////////////

	 int nodeIndex=id;
     int trailIndex=int(floor(nodeIndex/_nodeSegment));
     int nowIndexInNodes=int(id-trailIndex*_nodeSegment);
               
     node node_data0_1=outBuffer_node.nodes[CalCorrectIndex(trailIndex,int(nodeIndex-1))];
     node node_data00=outBuffer_node.nodes[CalCorrectIndex(trailIndex,int(nodeIndex))];
     node node_data01=outBuffer_node.nodes[CalCorrectIndex(trailIndex,int(nodeIndex+1))];
     node node_data02=outBuffer_node.nodes[CalCorrectIndex(trailIndex,int(nodeIndex+2))];
                
     vec3 node_pos=node_data00.node_position;
     vec3 node_nextPos=node_data01.node_position;
     vec3 node_dir=normalize(node_data01.node_position-node_data0_1.node_position);
     vec3 node_nextDir=normalize(node_data02.node_position-node_data00.node_position);
     float node_life=node_data00.node_life;

     trail now_trail_data=outBuffer_trail.trails[trailIndex];
     vec4 trail_color=now_trail_data.trail_color;
     if(nowIndexInNodes==now_trail_data.leftSideFirst||nowIndexInNodes==now_trail_data.rightSideFirst||nowIndexInNodes==now_trail_data.rightSideSecond){
        node_pos=node_pos;
        node_nextPos=node_pos;
        node_dir=node_pos;
        node_nextDir=node_pos;
     }

     v2g_o.node_pos=node_pos;
     v2g_o.node_nextPos=node_nextPos;
     v2g_o.node_dir=node_dir;
     v2g_o.node_nextDir=node_nextDir;
     v2g_o.node_life=node_life;
     v2g_o.trail_ID=trailIndex;
     v2g_o.trail_color=trail_color;
     v2g_o.nowIndexInNodes=nowIndexInNodes;

	///////////////////////////////////////////////////////

	gl_Position=vec4(vertex,1.0);
	
}