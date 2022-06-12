#include "Object.h"

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

#include "CNode.h"
#include "GraphicsEngine/Component/TransformComponent.h"
#include "../GraphicsMain/GraphicsMain.h"

Object::Object()
	: game(GraphicsMain::GetInstance()),
	m_RootNode(std::make_shared<obj::CNode>())
{
}

Object::~Object() {
}

void Object::Update() {
	m_RootNode->Update();
}

void Object::ProcessInput(const SDL_Event& e) {
	m_RootNode->ProcessInput(e);
}

void Object::SetTransform(glm::vec3 pos, glm::quat rot, glm::vec3 s)
{ 
	m_RootNode->GetTransform()->SetPosition(pos);
	m_RootNode->GetTransform()->SetRotation(rot);
	m_RootNode->GetTransform()->SetScale(s);

	m_RootNode->GetTransform()->CalMatrix();
}

void Object::SetPosition(glm::vec3 pos) {
	m_RootNode->GetTransform()->SetPosition(pos);

	m_RootNode->GetTransform()->CalMatrix();
}

void Object::SetRotation(glm::quat rot) {
	m_RootNode->GetTransform()->SetRotation(rot);

	m_RootNode->GetTransform()->CalMatrix();
}
void Object::SetScale(glm::vec3 s) {
	m_RootNode->GetTransform()->SetScale(s);

	m_RootNode->GetTransform()->CalMatrix();
}

glm::vec3 Object::GetPosition() {
	return m_RootNode->GetTransform()->GetPosition();
}

glm::quat Object::GetRotation() {
	return m_RootNode->GetTransform()->GetRotation();
}

glm::vec3 Object::GetScale() {
	return m_RootNode->GetTransform()->GetScale();
}

const std::shared_ptr<obj::CNode>& Object::GetRootNode()const {
	return m_RootNode;
}

std::shared_ptr<obj::CNode>& Object::GetRootNode() {
	return m_RootNode;
}