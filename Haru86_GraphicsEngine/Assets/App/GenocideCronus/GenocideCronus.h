#pragma once
#include <memory>

class GameObject;
class CTimeline;

namespace sound {
    class SoundShaderPlayer;
}

namespace myapp {
    class ProceduralCity;
    class SacredLake;
}

class GenocideCronus
{
    // Sound Shader
    std::shared_ptr<sound::SoundShaderPlayer> m_SoundPlayer;

    // デバッグ用
    std::shared_ptr<GameObject> m_GridPlane;
    
    // シーンオブジェクト
    std::shared_ptr<myapp::ProceduralCity> m_ProceduralCity;
    std::shared_ptr<myapp::SacredLake> m_SacredLake;

public:
    GenocideCronus() = default;
    ~GenocideCronus() = default;

    void Start();
    void Update();
    void Draw();
    void Timeline(CTimeline* timeline);
};

