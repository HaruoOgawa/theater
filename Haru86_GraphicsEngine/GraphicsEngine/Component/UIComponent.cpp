#include "UIComponent.h"
#include "../Object/GameObject.h"
#include "../GraphicsMain/GraphicsMain.h"

#include "../Graphics/GraphicsRenderer.h"
#include <glm/glm.hpp>
#include "./TransformComponent.h"
#include "./RectTransformComponent.h"
#include "../GraphicsMain/Time.h"
#include "../Object/TimelineObject.h"
#include "./TimelineComponent.h"
#include "../Component/AudioSourceComponent.h"
#include <algorithm>
#include "GraphicsEngine/Object/CNode.h"


UIComponent::UIComponent(GameObject* o)
	: Component(o)
{
	o->UseZTest(false);
	o->SetRenderOlder(static_cast<int>(RenderQueue::UI));
}

UIComponent::~UIComponent() {

}

void UIComponent::ProcessInput(const std::shared_ptr<app::CEventListener>& EventListener) {
	int x, y;
	//SDL_GetMouseState(&x, &y);

	/*float fx = static_cast<float>(x) - static_cast<float>(GraphicsRenderer::GetInstance()->GetScreenSize().x) * 0.5f;
	float fy = static_cast<float>(GraphicsRenderer::GetInstance()->GetScreenSize().y) * 0.5f- static_cast<float>(y);

	if (CheckInnnerBox(fx,fy)&& GraphicsMain::GetInstance()->mouseStateBool) {
		float maxLeftPointX = owner->GetRootNode()->
			GetComponent<RectTransformComponent>()->maxLeftPointX;
		float maxRightPointX = owner->GetRootNode()->
			GetComponent<RectTransformComponent>()->maxRightPointX;

		if (fx < maxLeftPointX) {
			fx = maxLeftPointX;
		}
		else if (fx>maxRightPointX) {
			fx = maxRightPointX;
		}

		float t = (maxLeftPointX-fx) / (maxLeftPointX-maxRightPointX);
		t = (t < 0.0f) ? 0.0f : t;
		// sample music time
		
		GraphicsMain::GetInstance()->timeObj->time = t;
		GraphicsMain::GetInstance()->previousTime = t;
		GraphicsMain::GetInstance()->time = t;
		
		float xval = fx / (static_cast<float>(GraphicsRenderer::GetInstance()->GetScreenSize().x) * 0.5f);
		float yval = fy / (static_cast<float>(GraphicsRenderer::GetInstance()->GetScreenSize().y) * 0.5f);
		
		glm::vec2 pos = glm::vec2(xval, yval);
		glm::vec4 modelPos =
			glm::inverse(owner->GetRootNode()->GetComponent<TransformComponent>()->GetPMatrix() * owner->GetRootNode()->GetComponent<TransformComponent>()->GetVMatrix())
			* glm::vec4(pos.x, pos.y, 0.0f, 1.0f);
		pos = glm::vec2(modelPos.x / modelPos.w, modelPos.y / modelPos.w);

		owner->SetPosition(glm::vec3(pos.x, owner->GetRootNode()->GetComponent<TransformComponent>()->GetPosition().y, owner->GetRootNode()->GetComponent<TransformComponent>()->GetPosition().z));
		owner->GetRootNode()->
			GetComponent<RectTransformComponent>()->rectPosition.x = fx;

		GraphicsMain::GetInstance()->timelineObj->timelineComponent->ResetTimeline();
		GraphicsMain::GetInstance()->timelineObj->timelineComponent->ComputeTimeline(t);
		std::sort(game->GetInstance()->gameObjectList.begin(), game->GetInstance()->gameObjectList.end(), [](GameObject* a, GameObject* b) {
			return a->renderOrder < b->renderOrder;
		});
	}*/
}

bool UIComponent::CheckInnnerBox( float x,  float y)const {
	glm::vec2 rectScale = owner->GetRootNode()->
		GetComponent<RectTransformComponent>()->rectScale;
	
	glm::vec2 rectPosition = owner->GetRootNode()->
		GetComponent<RectTransformComponent>()->rectPosition;

	glm::vec2 minPos = rectPosition - rectScale;
	glm::vec2 maxPos = rectPosition + rectScale;

	bool isInner = x< minPos.x ||
		y< minPos.y ||
		x>maxPos.x ||
		y>maxPos.y;

	return !isInner;
}

void UIComponent::Update() {

}