#pragma once
#include "Component.h"
#include <memory>

class UIComponent :
    public Component
{
public:
    UIComponent(class GameObject* o);
    ~UIComponent();
    void Update() override;
    void ProcessInput(const std::shared_ptr<app::CEventListener>& EventListener) override;
private:
    bool CheckInnnerBox( float x, float y)const;
};

