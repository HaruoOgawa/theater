#pragma once
#include <memory>

class GameObject;
class CTimeline;

namespace sound {
    class SoundShaderPlayer;
}

namespace myapp {
    class ProceduralCity;
}

class TheaterDemo
{
    //
    std::shared_ptr<sound::SoundShaderPlayer> m_SoundPlayer;

    //
    std::shared_ptr<GameObject> m_GridPlane;
    std::shared_ptr<GameObject> m_Raymarching;

    //
    std::shared_ptr<myapp::ProceduralCity> m_ProceduralCity;
public:
    TheaterDemo() = default;
    ~TheaterDemo() = default;

    void Start();
    void Update();
    void Draw();
    void Timeline(CTimeline* timeline);
};

