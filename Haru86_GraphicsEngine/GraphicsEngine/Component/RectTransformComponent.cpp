#include "RectTransformComponent.h"
#include "../Object/GameObject.h"
#include "../Graphics/GraphicsRenderer.h"
#include "./TransformComponent.h"
#include <iostream>
#include <string>
#include <glm/glm.hpp>

RectTransformComponent::RectTransformComponent(GameObject* o, float wRate, float hRate)
	:Component(o)
{
	
	glm::vec2 size = static_cast<glm::vec2>(GraphicsRenderer::GetInstance()->GetScreenSize());
	//rate 0.0 ~ 1.0
 	glm::vec2 pos = glm::vec2(size.x * wRate -size.x * 0.5, size.y * hRate - size.y * 0.5);
	rectPosition = pos;

	pos = pos / (size * 0.5f);

	glm::vec4 modelPos = 
		glm::inverse(owner->GetComponent<TransformComponent>()->GetPMatrix() *owner->GetComponent<TransformComponent>()->GetVMatrix())
		* glm::vec4(pos.x, pos.y, 0.0f, 1.0f);
	pos = glm::vec2(modelPos.x/ modelPos.w, modelPos.y/ modelPos.w);
	owner->SetPosition(glm::vec3(pos.x, pos.y, modelPos.z / modelPos.w));

	glm::vec4 uiScreenScale =  owner->GetComponent<TransformComponent>()->GetVMatrix() * glm::vec4(owner->GetComponent<TransformComponent>()->GetScale(), 1.0f);
	rectScale = glm::vec2(glm::abs(uiScreenScale.x)*size.x*0.5, glm::abs(uiScreenScale.y) * size.y*0.5);

	float val = 0.05f;
	maxLeftPointX = -size.x * 0.5 + size.x * val;
	maxRightPointX = size.x * 0.5 -size.x* val;
}

RectTransformComponent::~RectTransformComponent() {

}

void RectTransformComponent::ProcessInput(const std::shared_ptr<app::CEventListener>& EventListener) {

}