#include "TheaterDemo.h"

#include "GraphicsEngine/Object/TimelineObject.h"
#include "GraphicsEngine/Object/GameObject.h"
#include "GraphicsEngine/Graphics/ShaderLib.h"
#include "GraphicsEngine/Message/Console.h"

#define PI 3.14159265f

void TheaterDemo::Start() {
	// ”wŒiF
	GraphicsRenderer::GetInstance()->SetBackgroudColor(glm::vec4(glm::vec3(0.85f), 1.0));

	m_AnimObj = std::make_shared<GameObject>(PrimitiveType::CUBE);

	//
	m_GridPlane = std::make_shared<GameObject>(
		PrimitiveType::BOARD,
		RenderType::DefaultBuffer,
		RenderQueue::Geometry,
		shaderlib::ShaderLib::Standard_vert,
		shaderlib::ShaderLib::GridPlane_frag
		);
	m_GridPlane->SetRotation(glm::vec3(PI / 2.0f, 0.0, 0.0));
	m_GridPlane->SetPosition(glm::vec3(0.0f, 0.0f, 0.0f));
	m_GridPlane->SetScale(glm::vec3(25.0f));
}

void TheaterDemo::Update() {

}

void TheaterDemo::Draw() {

}

void TheaterDemo::Timeline(class TimelineObject* timelineObj) {

}