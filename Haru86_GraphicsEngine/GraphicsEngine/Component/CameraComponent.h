#pragma once
#include "Component.h"
#include <glm/glm.hpp>
#include <memory>
#include <glm/glm.hpp>

class CameraComponent :
    public Component
{
    int m_XMove;
    int m_YMove;
    int m_WheelX;
    int m_WheelY;

    bool m_IsRightClicked;
    bool m_IsLeftClicked;
    bool m_IsMouseWheel;
    
    glm::vec2 m_ScreenSize;
public:
    CameraComponent(class Object* o, enum class CameraType cameraType);
    ~CameraComponent();
    void Start() override;
    void Update() override;
    void ProcessInput(const std::shared_ptr<app::CEventListener>& EventListener) override;
    enum class CameraType m_cameraType;
    glm::vec3 cameraCenter;
    glm::vec3 cameraUp;
};

