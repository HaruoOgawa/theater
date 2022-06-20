#include "Object.h"

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

#include "GraphicsEngine/Component/Component.h"
#include "GraphicsEngine/Component/TransformComponent.h"
#include "GraphicsEngine/Component/MeshRendererComponent.h"
#include "GraphicsEngine/App/CEventListener.h"
#include "../GraphicsMain/GraphicsMain.h"

Object::Object()
	: game(GraphicsMain::GetInstance())
{
	m_transform = std::make_shared<TransformComponent>(nullptr, glm::vec3(0, 0, 0), glm::quat(glm::vec3(0, 0, 0)), glm::vec3(1.0f, 1.0f, 1.0f));
	AddComponent<TransformComponent>(m_transform);
}

Object::~Object() {
	m_ComponentList.clear();
}

void Object::Update() {
	for (auto comp : m_ComponentList) {
		comp.second->Update();
	}
}

void Object::ProcessInput(const std::shared_ptr<app::CEventListener>& EventListener) {
	for (auto comp : m_ComponentList) {
		comp.second->ProcessInput(EventListener);
	}
}

void Object::SetTransform(glm::vec3 pos, glm::quat rot, glm::vec3 s)
{ 
	m_transform->SetPosition(pos);
	m_transform->SetRotation(rot);
	m_transform->SetScale(s);

	m_transform->CalMatrix();
}

void Object::SetPosition(glm::vec3 pos) {
	m_transform->SetPosition(pos);

	m_transform->CalMatrix();
}

void Object::SetRotation(glm::quat rot) {
	m_transform->SetRotation(rot);

	m_transform->CalMatrix();
}
void Object::SetScale(glm::vec3 s) {
	m_transform->SetScale(s);

	m_transform->CalMatrix();
}

glm::vec3 Object::GetPosition() {
	return m_transform->GetPosition();
}

glm::quat Object::GetRotation() {
	return m_transform->GetRotation();
}

glm::vec3 Object::GetScale() {
	return m_transform->GetScale();
}

const std::unordered_map<std::type_index, std::shared_ptr<Component>>& Object::GetComponentList()const {
	return m_ComponentList;
}