#pragma once
#include "GraphicsEngine/Component/Component.h"
#include <memory>

struct trail {
    int nextCalNodeIndex;
    int leftSideFirst;
    int rightSideFirst;
    int rightSideSecond;
    float trail_color[4];
    trail(int nodeIndex,int nodeSegment) {
        nextCalNodeIndex= nodeIndex;
        leftSideFirst= 0;
        rightSideFirst= nodeSegment-1;
        rightSideSecond= nodeSegment-2;
        trail_color[0]= 1.0;
        trail_color[1]= 1.0;
        trail_color[2]= 1.0;
        trail_color[3]= 1.0;
    }
};

struct node {
    float node_position[3];
    float node_life;
    int renderFlag;
    node(float pos[3],float life) {
        node_position[0] = pos[0];
        node_position[1] = pos[1];
        node_position[2] = pos[2];
        node_life = life;
        renderFlag = 0;
    }
};

struct input_data {
    float nextInputPosition[3];
    input_data(float pos[3]) {
        nextInputPosition[0] = pos[0];
        nextInputPosition[1] = pos[1];
        nextInputPosition[2] = pos[2];
    }
};

class KuraTrail : 
	public Component
{
public:
    KuraTrail(class Object* o, class KuraGPGPU* kuraGPGPU);
    ~KuraTrail();
    void Start() override;
    void Update() override;
    void ProcessInput(const SDL_Event& e) override;
    void Draw();
private:
    class KuraGPGPU* kuraGPGPU_instance;
    std::shared_ptr<class Material> m_material;
    std::shared_ptr<class Mesh> m_mesh;
    std::shared_ptr<class ComputeBuffer> m_buffer_trail;
    std::shared_ptr<class ComputeBuffer> m_buffer_node;
    std::shared_ptr<class ComputeBuffer> m_buffer_input;
    std::shared_ptr<class ComputeShader> m_computeShader_NextInputPos;
    std::shared_ptr<class ComputeShader> m_computeShader_NodeInfo;
    int trailNum;
    int nodeNum;
    int nodeSegment;
    std::shared_ptr<class Texture> m_texture_trail;
};

