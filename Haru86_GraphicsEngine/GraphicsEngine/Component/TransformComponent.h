#pragma once
#include <glm/glm.hpp>
#include "../GraphicsMain/GraphicsMain.h"

class TransformComponent
{
    glm::vec3 m_position;
    glm::vec3 m_rotation;
    glm::vec3 m_scale;
    glm::mat4 m_mMatrix;
    glm::mat4 m_vMatrix;
    glm::mat4 m_pMatrix;
public:
    TransformComponent(class GameObject* o, glm::vec3 pos, glm::vec3 rot, glm::vec3 s);
    ~TransformComponent() = default;
    
    glm::mat4 GetMVPMatrix(){ return m_pMatrix * m_vMatrix * m_mMatrix; }
    glm::mat4 GetMMatrix(){ return m_mMatrix; }
    glm::mat4 GetInvMMatrix(){ return glm::inverse(m_mMatrix); }
    glm::mat4 GetVMatrix() { return m_vMatrix; }
    glm::mat4 GetPMatrix(){ return m_pMatrix; }
    const glm::vec3& GetPosition()const{ return m_position; }
    const glm::vec3& GetRotation()const{ return glm::vec3(m_rotation.x, m_rotation.y, m_rotation.z); }
    const glm::vec3& GetScale()const{ return m_scale; }

    void SetPosition(const glm::vec3& position){ m_position = position; }
    void SetRotation(const glm::vec3& rotation){ m_rotation = rotation; }
    void SetScale(const glm::vec3& scale){ m_scale = scale; }

    void CalMatrix();
    void ComputeModelMatrix();
    void ComputeViewMatrix();
    void ComputePerspectiveMatrix();

    friend GameObject;

private:
    class GameObject* owner;
    class GraphicsMain* game;
};

