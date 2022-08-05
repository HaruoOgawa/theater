#pragma once
#include <memory>

class GameObject;
class CTimeline;
class TransformComponent;

namespace myapp {
    class ProceduralCity;
    class SacredLake;
    class Forest;
    class Mountain;
    class SSR_Test;
}

class GenocideCronus
{
    // カメラ
    std::shared_ptr<TransformComponent> m_CameraTransform;


    // デバッグ用
    std::shared_ptr<GameObject> m_GridPlane;
    unsigned int m_DebugSceneID;
    
    // シーンオブジェクト
    std::shared_ptr<myapp::ProceduralCity> m_ProceduralCity;
    std::shared_ptr<myapp::SacredLake> m_SacredLake;
    std::shared_ptr<myapp::Forest> m_Forest;
    std::shared_ptr<myapp::Mountain> m_Mountain;

    std::shared_ptr<myapp::SSR_Test> m_SSR_Test;

public:
    GenocideCronus() = default;
    ~GenocideCronus() = default;

    void Start();
    void Update();
    void Draw();
    void Timeline(CTimeline* timeline);
};

