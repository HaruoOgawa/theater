#pragma once
#include <memory>

class GameObject;
class RaymarchingObject;

class TheaterDemo
{
    std::shared_ptr<GameObject> m_AnimObj;
    std::shared_ptr<GameObject> m_GridPlane;
    std::shared_ptr<RaymarchingObject> m_SampleRaymarchingObj;

public:
    TheaterDemo() = default;
    ~TheaterDemo() = default;

    void Start();
    void Update();
    void Draw();
    void Timeline(class TimelineObject* timelineObj);
};

