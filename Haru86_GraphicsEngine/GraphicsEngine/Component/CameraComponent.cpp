#include "CameraComponent.h"
#include "GraphicsEngine/Object/CameraObject.h"

#include "GraphicsEngine/Component/TransformComponent.h"
#include "GraphicsEngine/Graphics/GraphicsRenderer.h"
#include "GraphicsEngine/GraphicsMain/GraphicsMain.h"
#include "GraphicsEngine/Object/CNode.h"

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
		owner->GetRootNode()->GetComponent<TransformComponent>()->Rotate(5.0f*m_YMove/ m_ScreenSize.y,-5.0f*m_XMove/ m_ScreenSize.x,0.0f);
	}

	if (m_IsMouseWheel) {
		glm::vec3 viewDir = glm::normalize(cameraCenter - owner->GetRootNode()->GetComponent<TransformComponent>()->GetPosition());
		float zoomVal = ZoomPower*(float)m_WheelY;
		owner->GetRootNode()->GetComponent<TransformComponent>()->Translate(zoomVal * viewDir);
	}
}

void CameraComponent::ProcessInput(const std::shared_ptr<app::CEventListener>& EventListener) {
	// 前回のフレームからのマウスの移動距離を取得する
	/*int XMove, YMove = 0;
	Uint32 MouseState=SDL_GetRelativeMouseState(&m_XMove, &m_YMove);

	switch (MouseState)
	{
	case 1:
		m_IsRightClicked = false;
		break;
	
	case 4:
		m_IsRightClicked = true;
		break;

	case 2:
		m_IsRightClicked = false;
		break;

	default:
		m_IsRightClicked = false;
		break;
	}*/

	// マウスホイールの回転量を取得する
	/*if (e.type == SDL_MOUSEWHEEL) {
		m_IsMouseWheel = true;
		m_WheelX = static_cast<int>(e.wheel.x);
		m_WheelY = static_cast<int>(e.wheel.y);
	}
	else {
		m_IsMouseWheel = false;
	}*/
}