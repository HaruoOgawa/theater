#pragma once
#include <memory>
#include "GraphicsEngine/App/BaseApp/BaseApp.h"

class GameObject;

class AnimationLearning :
    public BaseApp
{
	std::shared_ptr<GameObject> m_AnimObj;
	std::shared_ptr<GameObject> m_GridPlane;
public:
	AnimationLearning() = default;
	~AnimationLearning() = default;

	virtual void Start() override;
	virtual void Update() override;
	virtual void Draw() override;
	virtual void Timeline(class TimelineObject* timelineObj) override;
};

