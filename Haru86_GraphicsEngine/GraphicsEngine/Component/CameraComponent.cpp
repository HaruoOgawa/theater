#include "CameraComponent.h"
#include "GraphicsEngine/Object/CameraObject.h"

#include "GraphicsEngine/Component/TransformComponent.h"
#include "GraphicsEngine/Graphics/GraphicsRenderer.h"
#include "GraphicsEngine/GraphicsMain/GraphicsMain.h"

#define ZoomPower 2.0f

CameraComponent::CameraComponent(Object* o, enum class CameraType cameraType)
	: Component(o)
	, m_cameraType(cameraType), cameraCenter(glm::vec3(0.0f, 0.0f, 0.0f)),cameraUp(glm::vec3(0.0f, 1.0f, 0.0f)),
	m_XMove(0),
	m_YMove(0),
	m_IsRightClicked(false),
	m_IsLeftClicked(false),
	m_ScreenSize(GraphicsRenderer::GetInstance()->GetScreenSize()),
	m_WheelX(0),
	m_WheelY(0),
	m_IsMouseWheel(false)
{

}

CameraComponent::~CameraComponent() {
}

void CameraComponent::Start() {

}

void CameraComponent::Update() {
	if (m_IsRightClicked) {
		owner->GetComponent<TransformComponent>()->Rotate(5.0f*m_YMove/ m_ScreenSize.y,-5.0f*m_XMove/ m_ScreenSize.x,0.0f);
	}

	if (m_IsMouseWheel) {
		glm::vec3 viewDir = glm::normalize(cameraCenter - owner->GetComponent<TransformComponent>()->GetPosition());
		float zoomVal = ZoomPower*(float)m_WheelY;
		owner->GetComponent<TransformComponent>()->Translate(zoomVal * viewDir);
	}
}