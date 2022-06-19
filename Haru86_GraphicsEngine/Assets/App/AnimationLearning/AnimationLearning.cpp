#include "AnimationLearning.h"
#include "GraphicsEngine/Object/TimelineObject.h"
#include "GraphicsEngine/Object/GameObject.h"
#include "GraphicsEngine/Graphics/ShaderLib.h"
#include "GraphicsEngine/Message/Console.h"

#define PI 3.14159265f

void AnimationLearning::Start() {
	/*Console::Log("shaderlib::ShaderLib::Standard_vert: %s\n", shaderlib::ShaderLib::Standard_vert.c_str());
	Console::Log("___________________________________\n");
	Console::Log("shaderlib::ShaderLib::Standard_frag: %s\n", shaderlib::ShaderLib::Standard_frag.c_str());
	Console::Log("shaderlib::ShaderLib::GridPlane_frag: %s\n", shaderlib::ShaderLib::GridPlane_frag.c_str());*/

	// ”wŒiF
	GraphicsRenderer::GetInstance()->SetBackgroudColor(glm::vec4(glm::vec3(0.85f), 1.0));
	
	m_AnimObj = std::make_shared<GameObject>(PrimitiveType::CUBE);

	//
	m_GridPlane = std::make_shared<GameObject>(
		PrimitiveType::BOARD,
		shaderlib::ShaderLib::Standard_vert,
		shaderlib::ShaderLib::GridPlane_frag
	);
	m_GridPlane->SetRotation(glm::vec3(PI/2.0f, 0.0, 0.0));
	m_GridPlane->SetPosition(glm::vec3(0.0f,0.0f,0.0f));
	m_GridPlane->SetScale(glm::vec3(25.0f));
}

void AnimationLearning::Update() {

}

void AnimationLearning::Draw() {

}

void AnimationLearning::Timeline(class TimelineObject* timelineObj) {

}