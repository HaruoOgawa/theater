#include "CameraObject.h"
#include "GraphicsEngine/Component/CameraComponent.h"
#include "GraphicsEngine/GraphicsMain/GraphicsMain.h"

CameraObject::CameraObject(CameraType cameraType)
	: Object(),m_cameraType(cameraType)
{
	m_cameraComponent =  std::make_shared<CameraComponent>(this,cameraType);
	AddComponent<CameraComponent>(m_cameraComponent);
}

CameraObject::~CameraObject() {
}