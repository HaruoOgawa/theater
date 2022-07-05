#include "GenocideCronus.h"

#include "GraphicsEngine/GraphicsMain/CTimeline.h"
#include "GraphicsEngine/Object/GameObject.h"
#include "GraphicsEngine/Graphics/ShaderLib.h"
#include "GraphicsEngine/Component/TransformComponent.h"
#include "GraphicsEngine/Sound/SoundShaderPlayer.h"
#include "GraphicsEngine/Graphics/GraphicsRenderer.h"
#include "GraphicsEngine/Message/Console.h"
#include "GraphicsEngine/Graphics/PostProcess.h"

#include "Assets/App/GenocideCronus/Script/ProceduralCity.h"
#include "Assets/App/GenocideCronus/Script/SacredLake.h"

void GenocideCronus::Start() {
	
	// カメラ
	//m_CameraTransform = std::make_shared<TransformComponent>(glm::vec3(-4.0f, 1.1f, -4.0f), glm::vec3(0.0f), glm::vec3(1.0f));
	m_CameraTransform = std::make_shared<TransformComponent>(glm::vec3(0.0f, 10.0f, 10.0f), glm::vec3(0.0f), glm::vec3(1.0f));
	//m_CameraTransform->m_center = glm::vec3(1.0f,6.0f,0.0f);
	m_CameraTransform->m_center = glm::vec3(0.0f,0.0f,0.0f);
	GraphicsMain::GetInstance()->m_CameraTransformList.push_back(m_CameraTransform);

	// サウンド
	std::string soundCode = {
		#include "Shader/Sound.frag"
	};
	m_SoundPlayer = std::make_shared<sound::SoundShaderPlayer>(soundCode);
	m_SoundPlayer->Initialize();

	// 背景色
	GraphicsRenderer::GetInstance()->SetBackgroudColor(glm::vec4(glm::vec3(0.4f, 0.6f, 1.0f), 1.0));
	
//#ifdef _DEBUG
	// デバッグ用グリッド
	/*m_GridPlane = std::make_shared<GameObject>(
		PrimitiveType::BOARD,
		RenderType::DefaultBuffer,
		RenderQueue::Geometry,
		RenderingSurfaceType::RASTERIZER,
		shaderlib::ShaderLib::Standard_vert,
		shaderlib::ShaderLib::GridPlane_frag
		);
	m_GridPlane->m_transform->m_rotation = glm::vec3(3.14159265f / 2.0f, 0.0, 0.0);
	m_GridPlane->m_transform->m_scale = glm::vec3(25.0f);*/
//#endif // _DEBUG

	// シーンオブジェクトの初期化
	//m_ProceduralCity = std::make_shared<myapp::ProceduralCity>();
	m_SacredLake = std::make_shared<myapp::SacredLake>();

	// ポストプロセスの設定
	PostProcess::GetInstance()->m_UsePostProcess = true;
	PostProcess::GetInstance()->m_UseBloom = true;
	PostProcess::GetInstance()->m_BloomThreshold = 1.0f;
	PostProcess::GetInstance()->m_BloomIntensity = 1.5f;

}

void GenocideCronus::Update() {
	// カメラ
	/*GraphicsMain::GetInstance()->m_CameraTransform->m_position = glm::vec3(
		glm::cos(GraphicsMain::GetInstance()->time*0.001f)*2.0f,
		2.5f,
		glm::sin(GraphicsMain::GetInstance()->time*0.001f)*2.0f
	);*/
	/*m_CameraTransform->m_position = glm::vec3(
		glm::cos(GraphicsMain::GetInstance()->time*0.001f)*20.0f,
		2.5f,
		glm::sin(GraphicsMain::GetInstance()->time*0.001f)*20.0f
	);*/
	m_CameraTransform->m_position = glm::vec3(
		glm::cos(GraphicsMain::GetInstance()->time * 0.001f) * 30.0f,
		10.0f,
		glm::sin(GraphicsMain::GetInstance()->time * 0.001f) * 30.0f
	);

	// サウンド
	m_SoundPlayer->Update();
}

void GenocideCronus::Draw() {
	//
	//m_ProceduralCity->Draw();
	m_SacredLake->Draw();
}

void GenocideCronus::Timeline(CTimeline* timeline) {

}