#pragma once
#include <glm/glm.hpp>
#include "../GraphicsMain/GraphicsMain.h"

class TransformComponent
{
    
public:
    TransformComponent(class GameObject* o, glm::vec3 pos, glm::vec3 rot, glm::vec3 s);
    ~TransformComponent() = default;
    
    void CalMatrix();
    void ComputeModelMatrix();
    void ComputeViewMatrix();
    void ComputePerspectiveMatrix();

    friend GameObject;

    glm::vec3 m_position;
    glm::vec3 m_rotation;
    glm::vec3 m_scale;
    glm::mat4 m_mMatrix;
    glm::mat4 m_vMatrix;
    glm::mat4 m_pMatrix;

private:
    class GameObject* owner;
    class GraphicsMain* game;
};

