#include "TimelineObject.h"
#include "../Component/TimelineComponent.h"
#include "GraphicsEngine/Object/CNode.h"


TimelineObject::TimelineObject() {
	timelineComponent =  std::make_shared<TimelineComponent>(this);
	GetRootNode()->AddComponent<TimelineComponent>(timelineComponent);
}
TimelineObject::~TimelineObject() {
}

void TimelineObject::AddActivationClip(GameObject* obj, float startTime, float endTime) {
	if (endTime < startTime) {
		return;
	}
	else {
		timelineComponent->waittingActiovationClip.emplace_back(new TimelineActivationClip(obj, startTime * 1000.0f, endTime * 1000.0f));
	}
}

void TimelineObject::AddCustomClip(GameObject* obj, float startTime, float endTime, std::function<void(void)> callback) {
	timelineComponent->waittingCustomClip.emplace_back(new TimelineCustomClip(obj,startTime*1000.0f,endTime*1000.0f,callback));
	
}

void TimelineObject::AddAnimationClip(GameObject* obj, float startTime, float endTime, std::function<void(float)> callback) {
	timelineComponent->waittingAnimationClip.emplace_back(new TimelineAnimationClip(obj,startTime*1000.0f,endTime*1000.0f,callback));
	
}

void TimelineObject::Initialize() {
	timelineComponent->Initialize();
}