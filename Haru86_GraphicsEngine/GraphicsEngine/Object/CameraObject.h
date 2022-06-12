#pragma once
#include "Object.h"
#include <memory>

enum class CameraType
{
    FIXED_CAMERA,
    FPS_CAMERA,
    FOLLOW_CAMERA
};

class CameraObject :
    public Object
{
public:
    CameraObject(CameraType cameraType);
    ~CameraObject();
    CameraType m_cameraType;
    std::shared_ptr<class CameraComponent> m_cameraComponent;
private:
    
};

