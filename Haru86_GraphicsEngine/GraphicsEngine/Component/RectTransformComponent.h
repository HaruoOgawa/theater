#pragma once
#include "Component.h"
#include <glm/glm.hpp>
#include <memory>

class RectTransformComponent :
    public Component
{
public:
    RectTransformComponent(class GameObject* o,float wRate,float hRate);
    ~RectTransformComponent();
    void ProcessInput(const std::shared_ptr<app::CEventListener>& EventListener) override;
    glm::vec2 rectPosition;
    glm::vec2 rectScale;
    float maxLeftPointX;
    float maxRightPointX;
};

