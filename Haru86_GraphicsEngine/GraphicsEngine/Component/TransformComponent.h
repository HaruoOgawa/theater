#pragma once
#include "./Component.h"
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include "../GraphicsMain/GraphicsMain.h"
#include <memory>

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
    TransformComponent();
    TransformComponent(class Object* o, glm::vec3 pos, glm::quat rot, glm::vec3 s);
    TransformComponent(class Object* o, const glm::mat4& WorldMatrix);
    ~TransformComponent();
    void Update() override;
    
    glm::mat4 GetMVPMatrix(){ return m_pMatrix * m_vMatrix * m_mMatrix; }
    glm::mat4 GetMMatrix(){ return m_mMatrix; }
    glm::mat4 GetInvMMatrix(){ return glm::inverse(m_mMatrix); }
    glm::mat4 GetVMatrix() { return m_vMatrix; }
    glm::mat4 GetPMatrix(){ return m_pMatrix; }
    const glm::vec3& GetPosition()const;
    const glm::quat& GetRotation()const;
    const glm::vec3& GetScale()const;

    void SetPosition(const glm::vec3& position);
    void SetRotation(const glm::quat& rotation);
    void SetScale(const glm::vec3& scale);

    void CalMatrix();
    void Translate(glm::vec3 MoveV);
    void Rotate(float XRot, float YRot, float ZRot);
    void ComputeModelMatrix();
    void ComputeViewMatrix();
    void ComputePerspectiveMatrix();
    void ProcessInput(const std::shared_ptr<app::CEventListener>& EventListener) override;

    static std::shared_ptr<TransformComponent> Combine(const std::shared_ptr<TransformComponent>& a, const std::shared_ptr<TransformComponent>& b);
    static std::shared_ptr<TransformComponent> inverse(const std::shared_ptr<TransformComponent>& t);
private:
   
};

