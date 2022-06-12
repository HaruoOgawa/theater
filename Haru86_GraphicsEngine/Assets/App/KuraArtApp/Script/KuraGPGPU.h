#pragma once
#include "GraphicsEngine/Component/Component.h"
#include <memory>
#include <glm/glm.hpp>

struct BoidsForce {
    float velocity[3];
    BoidsForce(float v[3]) {
        velocity[0] = v[0];
        velocity[1] = v[1];
        velocity[2] = v[2];
    }
};

struct BoidsData {
    float position[3];
    float velocity[3];
    float viewDir[3];
    BoidsData(float p[3],float v[3]) {
        position[0] = p[0];
        position[1] = p[1];
        position[2] = p[2];

        velocity[0] = v[0];
        velocity[1] = v[1];
        velocity[2] = v[2];

        viewDir[0] = p[0];
        viewDir[1] = p[1];
        viewDir[2] = p[2];

    }
};

class KuraGPGPU :
    public Component
{
public:
    KuraGPGPU(class Object* o);
    ~KuraGPGPU();
    void Start() override;
    void Update() override;
    void ProcessInput(const SDL_Event& e) override;
    void Draw();
    float rand(glm::vec2 st);
    int num;
    std::shared_ptr<class ComputeBuffer> m_computeBuffer_BoidsData;
    friend class KuraArtApp;
private:
    std::shared_ptr<class Material> m_material;
    std::shared_ptr<class Mesh> m_mesh;
    
    std::shared_ptr<class ComputeBuffer> m_computeBuffer_BoidsForce;
    std::shared_ptr<class ComputeShader> m_computeShader_force;
    std::shared_ptr<class ComputeShader> m_computeShader_data;
   
    
};

