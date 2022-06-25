#pragma once

#include <vector>
#include <functional>
#include <memory>

struct TimelineActivationClip
{
    class GameObject* gameObject;
    float startTime;
    float endTime;
    TimelineActivationClip(class GameObject* obj, float s, float e) {
        this->gameObject = obj;
        this->startTime = s;
        this->endTime = e;
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
    TimelineAnimationClip(class GameObject* obj, float s, float e, std::function<void(float)> func) {
        this->gameObject = obj;
        this->startTime = s;
        this->endTime = e;
        this->callback = func;
        this->lifeTime = 0.0f;
        this->lifeTimeRate = 0.0f;
    }
};

class CTimeline
{
    std::vector<class TimelineActivationClip*> waittingActiovationClip;
    std::vector<class TimelineActivationClip*> runningActiovationClip;
    std::vector<class TimelineAnimationClip*> waittingAnimationClip;
    std::vector<class TimelineAnimationClip*> runningAnimationClip;
    float timeVal;
    float animTime;
public:
    CTimeline();
    ~CTimeline()=default;

    void AddActivationClip(class GameObject* obj, float startTime, float endTime);
    void AddAnimationClip(class GameObject* obj, float startTime, float endTime, std::function<void(float)> callback);

    void Update();
    void Initialize();
    void ComputeTimeline(float time);
    void ResetTimeline();

private:
    void InitializeActivationTrack();
    void InitializeAnimationTrack();
    void ComputeActivationTrack(float time);
    void ComputeAnimationTrack(float time);
};

