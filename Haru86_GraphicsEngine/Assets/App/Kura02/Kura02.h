#pragma once
#include "../../../GraphicsEngine/GraphicsMain/GraphicsMain.h"
#include "../../../GraphicsEngine/App/BaseApp/BaseApp.h"
#include <memory>
class Kura02 :
    public BaseApp
{
public:
    Kura02();
    ~Kura02();
    void Start()  override;
    void Update()  override;
    void Draw() override;
    void Timeline(class TimelineObject* timelineObj)  override;
private:
    std::unique_ptr<class Object> ins_cubes;
    std::shared_ptr<class CameraObject> cam_obj;
    std::unique_ptr<class GameObject> test_obj;
    std::unique_ptr<class GameObject> test_obj2;
    std::unique_ptr<class RaymarchingObject> raymarching_obj;
};

