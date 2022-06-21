#include "TransformComponent.h"
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include "../GraphicsMain/GraphicsMain.h"
#include "../Graphics/GraphicsRenderer.h"
#include "GraphicsEngine/Object/CameraObject.h"
#include "GraphicsEngine/Component/CameraComponent.h"

TransformComponent::TransformComponent(Object* o, glm::vec3 pos,glm::quat rot,glm::vec3 s)
	: Component(o), m_position(pos), m_rotation(rot), m_scale(s)
{
	CalMatrix();
}

void TransformComponent::Translate(glm::vec3 MoveV) {
	m_position += MoveV;
}
void TransformComponent::Rotate(float XRot, float YRot, float ZRot) {
	glm::mat4 RotMatrix= glm::mat4_cast(glm::quat(glm::vec3(XRot, YRot, ZRot)));
	glm:: vec4 rotPosition = (RotMatrix * glm::vec4(m_position, 1.0));
	m_position = glm::vec3(rotPosition.x, rotPosition.y, rotPosition.z);
}

void TransformComponent::CalMatrix() {
	ComputeModelMatrix();
	ComputeViewMatrix();
	ComputePerspectiveMatrix();
}

void TransformComponent::ComputeModelMatrix() {
	glm::mat4 translationMatirx = glm::translate(glm::mat4(1.0f), glm::vec3(m_position.x, m_position.y, m_position.z));
	glm::mat4 rotationMatrix = glm::mat4_cast(m_rotation);
	glm::mat4 scaleMatrix = glm::scale(glm::mat4(1.0f),glm::vec3(m_scale.x, m_scale.y, m_scale.z));

	m_mMatrix = translationMatirx* rotationMatrix * scaleMatrix;
}

void TransformComponent::ComputeViewMatrix() {
	if (GraphicsMain::GetInstance()->game_camera_instance!=nullptr) {
		std::shared_ptr<CameraObject> cam = GraphicsMain::GetInstance()->game_camera_instance;
		m_vMatrix = glm::lookAt(
			cam->GetComponent<TransformComponent>()->m_position,
			cam->m_cameraComponent->cameraCenter,
			cam->m_cameraComponent->cameraUp
		);
	}
	else {
		m_vMatrix = glm::lookAt(
			glm::vec3(0.0f, 0.0f, -3.0f),
			glm::vec3(0.0f, 0.0f, 0.0f),
			glm::vec3(0.0f, 1.0f, 0.0f)
		);
	}
}

void TransformComponent::ComputePerspectiveMatrix() {
	float width = static_cast<float>(GraphicsRenderer::GetInstance()->GetScreenSize().x);
	float height = static_cast<float>(GraphicsRenderer::GetInstance()->GetScreenSize().y);
	
	m_pMatrix = glm::perspective(
		glm::radians(90.0f),
		width/height,
		0.1f,
		10000.0f
	);
}