#pragma once
#include "Object.h"
#include <functional>
#include <memory>

class TimelineObject :
    public Object
{
public:
    TimelineObject();
    ~TimelineObject();
    void AddActivationClip(class GameObject* obj,float startTime,float endTime);
    void AddCustomClip(class GameObject* obj,float startTime,float endTime,std::function<void(void)> callback);
    void AddAnimationClip(class GameObject* obj,float startTime,float endTime,std::function<void(float)> callback);
    std::shared_ptr<class TimelineComponent> timelineComponent;
    friend GraphicsMain;
    friend class UIComponent;
    
private:
    
    void Initialize();
};

