#pragma once
#include <memory>

class TransformComponent;

namespace myapp {
    class ProceduralCity;
    class SacredLake;
    class Forest;
    class Mountain;
}

class GenocideCronus
{
    // 基本パラメーター
    std::shared_ptr<TransformComponent> m_CameraTransform;
    unsigned int m_SceneIndex;
    float        m_LocalTime;

    float        m_LocalReverseTime;
    bool         m_IsReverseTime;

    // デバッグ用: 時よ止まれっっっっ！！！！！！
    bool m_DebugTimeLock;
  
    // シーンオブジェクト
    std::shared_ptr<myapp::ProceduralCity> m_ProceduralCity;
    std::shared_ptr<myapp::SacredLake> m_SacredLake;
    std::shared_ptr<myapp::Forest> m_Forest;
    std::shared_ptr<myapp::Mountain> m_Mountain;
public:
    GenocideCronus();
    ~GenocideCronus() = default;

    void Start();
    void Update();
    void Draw(bool IsRaymarching);
    void UpdateTimeline();

    unsigned int GetSceneIndex()const { return m_SceneIndex; }
};

