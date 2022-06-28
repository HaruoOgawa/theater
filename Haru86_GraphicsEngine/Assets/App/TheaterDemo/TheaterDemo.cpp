#include "TheaterDemo.h"

#include "GraphicsEngine/GraphicsMain/CTimeline.h"
#include "GraphicsEngine/Object/GameObject.h"
#include "GraphicsEngine/Graphics/ShaderLib.h"
#include "GraphicsEngine/Component/TransformComponent.h"
#include "GraphicsEngine/Sound/SoundShaderPlayer.h"
#include "Assets/App/TheaterDemo/Script/ProceduralCity.h"
#include "GraphicsEngine/Graphics/GraphicsRenderer.h"
#include "GraphicsEngine/Message/Console.h"

#define PI 3.0f

void TheaterDemo::Start() {
	// カメラ
	GraphicsMain::GetInstance()->m_CameraTransform = std::make_shared<TransformComponent>(glm::vec3(0.0f, 0.0f, -3.0f), glm::vec3(0.0f), glm::vec3(1.0f));

	// サウンド
	std::string soundCode = {
		#include "Shader/Sound.frag"
	};
	m_SoundPlayer = std::make_shared<sound::SoundShaderPlayer>(soundCode);
	m_SoundPlayer->Initialize();

	// 背景色
	GraphicsRenderer::GetInstance()->SetBackgroudColor(glm::vec4(glm::vec3(0.85f), 1.0));

	//
	m_ProceduralCity = std::make_shared<myapp::ProceduralCity>();

}

void TheaterDemo::Update() {
	// カメラ
	GraphicsMain::GetInstance()->m_CameraTransform->m_position = glm::vec3(
		glm::cos(GraphicsMain::GetInstance()->time*0.001f)*3.0f,
		0.0f,
		glm::sin(GraphicsMain::GetInstance()->time*0.001f)*3.0f
	);

	// サウンド
	m_SoundPlayer->Update();
}

void TheaterDemo::Draw() {
	//
	m_ProceduralCity->Draw();
}

void TheaterDemo::Timeline(CTimeline* timeline) {

}