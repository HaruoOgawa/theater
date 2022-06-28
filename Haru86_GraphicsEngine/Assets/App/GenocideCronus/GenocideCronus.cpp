#include "GenocideCronus.h"

#include "GraphicsEngine/GraphicsMain/CTimeline.h"
#include "GraphicsEngine/Object/GameObject.h"
#include "GraphicsEngine/Graphics/ShaderLib.h"
#include "GraphicsEngine/Component/TransformComponent.h"
#include "GraphicsEngine/Sound/SoundShaderPlayer.h"
#include "Assets/App/GenocideCronus/Script/ProceduralCity.h"
#include "GraphicsEngine/Graphics/GraphicsRenderer.h"
#include "GraphicsEngine/Message/Console.h"

#define PI 3.0f

void GenocideCronus::Start() {
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
	
	// デバッグ用グリッド
	m_GridPlane = std::make_shared<GameObject>(
		PrimitiveType::BOARD,
		RenderType::DefaultBuffer,
		RenderQueue::Geometry,
		RenderingSurfaceType::RASTERIZER,
		shaderlib::ShaderLib::Standard_vert,
		shaderlib::ShaderLib::GridPlane_frag
		);
	m_GridPlane->m_transform->m_rotation = glm::vec3(PI / 2.0f, 0.0, 0.0);
	m_GridPlane->m_transform->m_scale = glm::vec3(25.0f);

	//
	m_ProceduralCity = std::make_shared<myapp::ProceduralCity>();

}

void GenocideCronus::Update() {
	// カメラ
	GraphicsMain::GetInstance()->m_CameraTransform->m_position = glm::vec3(
		glm::cos(GraphicsMain::GetInstance()->time*0.001f)*3.0f,
		1.0f,
		glm::sin(GraphicsMain::GetInstance()->time*0.001f)*3.0f
	);

	// サウンド
	m_SoundPlayer->Update();
}

void GenocideCronus::Draw() {
	//
	m_ProceduralCity->Draw();
}

void GenocideCronus::Timeline(CTimeline* timeline) {

}