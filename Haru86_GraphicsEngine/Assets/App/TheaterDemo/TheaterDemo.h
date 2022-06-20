#pragma once
#include <memory>
#include "GraphicsEngine/App/BaseApp/BaseApp.h"

class GameObject;
class RaymarchingObject;

class TheaterDemo :
    public BaseApp
{
    std::shared_ptr<GameObject> m_AnimObj;
    std::shared_ptr<GameObject> m_GridPlane;
    //std::shared_ptr<RaymarchingObject> m_SampleRaymarchingObj;

public:
    TheaterDemo() = default;
    ~TheaterDemo() = default;

    virtual void Start() override;
    virtual void Update() override;
    virtual void Draw() override;
    virtual void Timeline(class TimelineObject* timelineObj) override;
};

