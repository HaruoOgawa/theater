#pragma once
#include "./Component.h"
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include "../GraphicsMain/GraphicsMain.h"

class TransformComponent :
    public Component
{
    glm::vec3 m_position;
    glm::quat m_rotation;
    glm::vec3 m_scale;
    glm::mat4 m_mMatrix;
    glm::mat4 m_vMatrix;
    glm::mat4 m_pMatrix;
public:
    TransformComponent(class Object* o, glm::vec3 pos, glm::quat rot, glm::vec3 s);
    ~TransformComponent() = default;
    void Update() override{}
    
    glm::mat4 GetMVPMatrix(){ return m_pMatrix * m_vMatrix * m_mMatrix; }
    glm::mat4 GetMMatrix(){ return m_mMatrix; }
    glm::mat4 GetInvMMatrix(){ return glm::inverse(m_mMatrix); }
    glm::mat4 GetVMatrix() { return m_vMatrix; }
    glm::mat4 GetPMatrix(){ return m_pMatrix; }
    const glm::vec3& GetPosition()const{ return m_position; }
    const glm::quat& GetRotation()const{ return m_rotation; }
    const glm::vec3& GetScale()const{ return m_scale; }

    void SetPosition(const glm::vec3& position){ m_position = position; }
    void SetRotation(const glm::quat& rotation){ m_rotation = rotation; }
    void SetScale(const glm::vec3& scale){ m_scale = scale; }

    void CalMatrix();
    void Translate(glm::vec3 MoveV);
    void Rotate(float XRot, float YRot, float ZRot);
    void ComputeModelMatrix();
    void ComputeViewMatrix();
    void ComputePerspectiveMatrix();
    void ProcessInput(const std::shared_ptr<app::CEventListener>& EventListener) override{}

private:
   
};

