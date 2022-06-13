#include "TransformComponent.h"
//#include <glm/glm.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include "../GraphicsMain/GraphicsMain.h"
#include "../Graphics/GraphicsRenderer.h"
#include "GraphicsEngine/Object/CameraObject.h"
#include "GraphicsEngine/Component/CameraComponent.h"
#include "GraphicsEngine/Object/CNode.h"

TransformComponent::TransformComponent() :
	Component(nullptr),
	m_position(glm::vec3(0.0)),
	m_rotation(glm::quat(glm::vec3(0.0))),
	m_scale(glm::vec3(1.0))
{
	CalMatrix();
}

TransformComponent::TransformComponent(Object* o, glm::vec3 pos,glm::quat rot,glm::vec3 s)
	: Component(o), m_position(pos), m_rotation(rot), m_scale(s)
{
	CalMatrix();
}

TransformComponent::TransformComponent(Object* o, const glm::mat4& WorldMatrix):
	Component(o),
	m_position(glm::vec3(0.0)),
	m_rotation(glm::quat(glm::vec3(0.0))),
	m_scale(glm::vec3(1.0))
{
	// mat4 to transform
	//
	/*m_position = glm::vec3(WorldMatrix[3][0], WorldMatrix[3][1], WorldMatrix[3][2]);

	//
	float sx = glm::length(glm::vec3(WorldMatrix[0][0], WorldMatrix[0][1], WorldMatrix[0][2]));
	float sy = glm::length(glm::vec3(WorldMatrix[1][0], WorldMatrix[1][1], WorldMatrix[1][2]));
	float sz = glm::length(glm::vec3(WorldMatrix[2][0], WorldMatrix[2][1], WorldMatrix[2][2]));
	m_scale = glm::vec3(sx,sy,sz);

	//
	glm::mat4 rot = glm::mat4(0.0);

	rot[0][0] = WorldMatrix[0][0] / sx;
	rot[0][1] = WorldMatrix[0][1] / sx;
	rot[0][2] = WorldMatrix[0][2] / sx;
	rot[0][3] = 0.0f;

	rot[1][0] = WorldMatrix[1][0] / sy;
	rot[1][1] = WorldMatrix[1][1] / sy;
	rot[1][2] = WorldMatrix[1][2] / sy;
	rot[1][3] = 0.0f;

	rot[2][0] = WorldMatrix[2][0] / sz;
	rot[2][1] = WorldMatrix[2][1] / sz;
	rot[2][2] = WorldMatrix[2][2] / sz;
	rot[2][3] = 0.0f;

	rot[3][0] = 0.0f;
	rot[3][1] = 0.0f;
	rot[3][2] = 0.0f;
	rot[3][3] = 1.0f;

	m_rotation = glm::quat_cast(rot);*/

	m_position = glm::vec3(WorldMatrix[3][0], WorldMatrix[3][1], WorldMatrix[3][2]);
	m_rotation = glm::quat_cast(WorldMatrix);

	glm::mat4 rotScaleMat(
		WorldMatrix[0][0], WorldMatrix[1][0], WorldMatrix[2][0], 0,
		WorldMatrix[0][1], WorldMatrix[1][1], WorldMatrix[2][1], 0,
		WorldMatrix[0][2], WorldMatrix[1][2], WorldMatrix[2][2], 0,
		0, 0, 0, 1
	);
	glm::mat4 invRotMat = glm::mat4_cast(glm::inverse(m_rotation));
	glm::mat4 scaleSkewMat = rotScaleMat * invRotMat;

	m_scale = glm::vec3(
		scaleSkewMat[0][0],
		scaleSkewMat[1][1],
		scaleSkewMat[2][2]
	);

	CalMatrix();
}

TransformComponent::~TransformComponent() {
}

void TransformComponent::Update() {

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
			cam->GetRootNode()->GetComponent<TransformComponent>()->m_position,
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

void TransformComponent::ProcessInput(const std::shared_ptr<app::CEventListener>& EventListener) {

}

std::shared_ptr<TransformComponent> TransformComponent::Combine(const std::shared_ptr<TransformComponent>& a, const std::shared_ptr<TransformComponent>& b) {
	std::shared_ptr<TransformComponent> result = std::make_shared<TransformComponent>();

	result->SetScale(a->GetScale()*b->GetScale());
	result->SetRotation(b->GetRotation()*a->GetRotation());
	
	result->SetPosition(a->GetRotation() * (a->GetScale() * b->GetPosition()));
	result->SetPosition(a->GetPosition()+result->GetPosition());

	result->CalMatrix();
	return result;
}

std::shared_ptr<TransformComponent> TransformComponent::inverse(const std::shared_ptr<TransformComponent>& t) {
	std::shared_ptr<TransformComponent> inv=std::make_shared<TransformComponent>();

	inv->SetRotation(glm::inverse(t->GetRotation()));

	inv->SetScale(glm::vec3(
		(glm::abs(t->GetScale().x)< 0.000001f)? 0.0f : 1.0f / t->GetScale().x,
		(glm::abs(t->GetScale().y)< 0.000001f)? 0.0f : 1.0f / t->GetScale().y,
		(glm::abs(t->GetScale().z)< 0.000001f)? 0.0f : 1.0f / t->GetScale().z
	));

	inv->SetPosition(
		inv->GetRotation()*(inv->GetScale()*(t->GetPosition()-1.0f))
	);
	inv->CalMatrix();
	return inv;
}

const glm::vec3& TransformComponent::GetPosition()const {
	return m_position;
}

const glm::quat& TransformComponent::GetRotation()const {
	return m_rotation;
}

const glm::vec3& TransformComponent::GetScale()const {
	return m_scale;
}

void TransformComponent::SetPosition(const glm::vec3& position) {
	m_position = position;
}

void TransformComponent::SetRotation(const glm::quat& rotation) {
	m_rotation = rotation;
}

void TransformComponent::SetScale(const glm::vec3& scale) {
	m_scale = scale;
}