#pragma once
#include "Component.h"
#include <vector>
#include <functional>
#include <memory>

struct TimelineActivationClip
{
    class GameObject* gameObject;
    float startTime;
    float endTime;
    TimelineActivationClip(class GameObject* obj,float s,float e) {
        this->gameObject = obj;
        this->startTime = s;
        this->endTime = e;
    }
};

struct TimelineCustomClip
{
    class GameObject* gameObject;
    float startTime;
    float endTime;
    std::function<void(void)> callback;
    TimelineCustomClip(class GameObject* obj,float s,float e,std::function<void(void)> func) {
        this->gameObject = obj;
        this->startTime = s;
        this->endTime = e;
        this->callback = func;
    }
};

struct TimelineAnimationClip
{
    class GameObject* gameObject;
    float startTime;
    float endTime;
    float lifeTime;
    float lifeTimeRate;
    std::function<void(float)> callback;
    TimelineAnimationClip(class GameObject* obj,float s,float e,std::function<void(float)> func) {
        this->gameObject = obj;
        this->startTime = s;
        this->endTime = e;
        this->callback = func;
        this->lifeTime = 0.0f;
        this->lifeTimeRate = 0.0f;
    }
};

class TimelineComponent :
    public Component
{
public:
    TimelineComponent(class Object* o);
    ~TimelineComponent();
    void Update() override;
    void Initialize();
    void ComputeTimeline(float time);
    void ResetTimeline();
    void ProcessInput(const SDL_Event& e) override;
    friend class TimelineObject;
    friend class UIComponent;
    float timeVal;
    float animTime;
private:
    void InitializeActivationTrack();
    void InitializeCustomTrack();
    void InitializeAnimationTrack();
    void ComputeActivationTrack(float time);
    void ComputeCustomTrack(float time);
    void ComputeAnimationTrack(float time);
    void ClearTimeline();
    std::vector<class TimelineActivationClip*> waittingActiovationClip;
    std::vector<class TimelineActivationClip*> runningActiovationClip;
    std::vector<class TimelineCustomClip*> waittingCustomClip;
    std::vector<class TimelineCustomClip*> runningCustomClip;
    std::vector<class TimelineAnimationClip*> waittingAnimationClip;
    std::vector<class TimelineAnimationClip*> runningAnimationClip;
};

