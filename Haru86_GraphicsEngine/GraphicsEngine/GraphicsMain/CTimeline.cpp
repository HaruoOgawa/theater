#include "CTimeline.h"
#include "../Object/GameObject.h"
#include "../Component/MeshRendererComponent.h"
#include "GraphicsEngine/GraphicsMain/GraphicsMain.h"

CTimeline::CTimeline()
{
	timeVal = GraphicsMain::GetInstance()->time;
	animTime = 0.0f;
}

void CTimeline::AddActivationClip(GameObject* obj, float startTime, float endTime) {
	if (endTime < startTime) {
		return;
	}
	else {
		waittingActiovationClip.emplace_back(new TimelineActivationClip(obj, startTime * 1000.0f, endTime * 1000.0f));
	}
}

void CTimeline::AddAnimationClip(GameObject* obj, float startTime, float endTime, std::function<void(float)> callback) {
	waittingAnimationClip.emplace_back(new TimelineAnimationClip(obj, startTime * 1000.0f, endTime * 1000.0f, callback));
}

void CTimeline::Update() {
	timeVal = GraphicsMain::GetInstance()->time;
	ComputeTimeline(timeVal);
}

void CTimeline::Initialize() {
	InitializeActivationTrack();
	InitializeAnimationTrack();
}

void CTimeline::InitializeActivationTrack() {
	for (auto clip : waittingActiovationClip) {
		auto iter = std::find(GraphicsMain::GetInstance()->GetInstance()->gameObjectList.begin(), GraphicsMain::GetInstance()->GetInstance()->gameObjectList.end(), clip->gameObject);
		if (iter != GraphicsMain::GetInstance()->GetInstance()->gameObjectList.end()) {
			GraphicsMain::GetInstance()->GetInstance()->gameObjectList.erase(GraphicsMain::GetInstance()->GetInstance()->gameObjectList.begin()
				+ std::distance(GraphicsMain::GetInstance()->GetInstance()->gameObjectList.begin(), iter));
		}
	}
}

void CTimeline::InitializeAnimationTrack() {

}

void CTimeline::ComputeTimeline(float time) {
	ComputeActivationTrack(time);
	ComputeAnimationTrack(time);
}

void CTimeline::ComputeActivationTrack(float time) {
	for (auto clip : waittingActiovationClip) {
		if (clip->startTime < time && clip->endTime >= time) {
			GraphicsMain::GetInstance()->GetInstance()->gameObjectList.emplace_back(clip->gameObject);
			runningActiovationClip.emplace_back(clip);

			auto waittingIter = std::find(waittingActiovationClip.begin(), waittingActiovationClip.end(), clip);
			if (waittingIter != waittingActiovationClip.end()) {
				waittingActiovationClip.erase(waittingActiovationClip.begin()
					+ std::distance(waittingActiovationClip.begin(), waittingIter)
				);
			}
		}
	}

	for (auto clip : runningActiovationClip) {
		if (time > clip->endTime) {
			auto iter = std::find(GraphicsMain::GetInstance()->GetInstance()->gameObjectList.begin(), GraphicsMain::GetInstance()->GetInstance()->gameObjectList.end(), clip->gameObject);
			if (iter != GraphicsMain::GetInstance()->GetInstance()->gameObjectList.end()) {
				GraphicsMain::GetInstance()->GetInstance()->gameObjectList.erase(
					GraphicsMain::GetInstance()->GetInstance()->gameObjectList.begin()
					+ std::distance(GraphicsMain::GetInstance()->GetInstance()->gameObjectList.begin(), iter)
				);
			}

			auto runningIter = std::find(runningActiovationClip.begin(), runningActiovationClip.end(), clip);
			if (runningIter != runningActiovationClip.end()) {
				runningActiovationClip.erase(runningActiovationClip.begin()
					+ std::distance(runningActiovationClip.begin(), runningIter)
				);
			}

			waittingActiovationClip.emplace_back(clip);

		}
	}
}

void CTimeline::ComputeAnimationTrack(float time) {
	for (auto clip : waittingAnimationClip) {
		if (clip->startTime < time && clip->endTime >= time) {
			clip->lifeTime = 0.0f;
			clip->lifeTimeRate = 0.0f;
			clip->gameObject->meshComp->animationClips.emplace_back(clip);
			runningAnimationClip.emplace_back(clip);

			auto waittingIter = std::find(waittingAnimationClip.begin(), waittingAnimationClip.end(), clip);
			if (waittingIter != waittingAnimationClip.end()) {
				waittingAnimationClip.erase(waittingAnimationClip.begin()
					+ std::distance(waittingAnimationClip.begin(), waittingIter)
				);
			}
		}
	}

	for (auto clip : runningAnimationClip) {
		clip->lifeTime += GraphicsMain::GetInstance()->deltaTime;
		clip->lifeTimeRate = clip->lifeTime / (clip->endTime * 0.001f - clip->startTime * 0.001f);
		if (time > clip->endTime) {
			auto iter = std::find(clip->gameObject->meshComp->animationClips.begin(), clip->gameObject->meshComp->animationClips.end(), clip);
			if (iter != clip->gameObject->meshComp->animationClips.end()) {
				clip->gameObject->meshComp->animationClips.erase(clip->gameObject->meshComp->animationClips.begin()
					+ std::distance(clip->gameObject->meshComp->animationClips.begin(), iter));
			}

			auto runningIter = std::find(runningAnimationClip.begin(), runningAnimationClip.end(), clip);
			if (runningIter != runningAnimationClip.end()) {
				runningAnimationClip.erase(runningAnimationClip.begin()
					+ std::distance(runningAnimationClip.begin(), runningIter)
				);
			}

			waittingAnimationClip.emplace_back(clip);

		}
	}
}

void CTimeline::ResetTimeline() {
	for (auto clip : runningActiovationClip) {
		auto iter = std::find(GraphicsMain::GetInstance()->GetInstance()->gameObjectList.begin(), GraphicsMain::GetInstance()->GetInstance()->gameObjectList.end(), clip->gameObject);
		if (iter != GraphicsMain::GetInstance()->GetInstance()->gameObjectList.end()) {
			GraphicsMain::GetInstance()->GetInstance()->gameObjectList.erase(GraphicsMain::GetInstance()->GetInstance()->gameObjectList.begin()
				+ std::distance(GraphicsMain::GetInstance()->GetInstance()->gameObjectList.begin(), iter));
		}

		waittingActiovationClip.emplace_back(clip);
	}
	runningActiovationClip.clear();

	for (auto clip : runningAnimationClip) {
		clip->lifeTime = 0.0f;
		clip->lifeTimeRate = 0.0f;
		clip->gameObject->meshComp->animationClips.clear();
		waittingAnimationClip.emplace_back(clip);
	}
	runningAnimationClip.clear();

}