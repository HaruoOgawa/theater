#include "GenocideCronus.h"

#include "GraphicsEngine/GraphicsMain/CTimeline.h"
#include "GraphicsEngine/Object/GameObject.h"
#include "GraphicsEngine/Graphics/ShaderLib.h"
#include "GraphicsEngine/Component/TransformComponent.h"
#include "GraphicsEngine/Graphics/GraphicsRenderer.h"
#include "GraphicsEngine/Message/Console.h"
#include "GraphicsEngine/Graphics/PostProcess.h"

#include "Assets/App/GenocideCronus/Script/ProceduralCity.h"
#include "Assets/App/GenocideCronus/Script/SacredLake.h"
#include "Assets/App/GenocideCronus/Script/Forest.h"

#include "Assets/App/GenocideCronus/Script/SSR_Test.h"

void GenocideCronus::Start() {
	
	// カメラ
	//m_CameraTransform = std::make_shared<TransformComponent>(glm::vec3(-4.0f, 1.1f, -4.0f), glm::vec3(0.0f), glm::vec3(1.0f));
	//m_CameraTransform = std::make_shared<TransformComponent>(glm::vec3(0.0f, 0.5f, 30.0f), glm::vec3(0.0f), glm::vec3(1.0f)); // SacredLake
	//m_CameraTransform = std::make_shared<TransformComponent>(glm::vec3(0.0f, 10.0f, 10.0f), glm::vec3(0.0f), glm::vec3(1.0f));
	//m_CameraTransform = std::make_shared<TransformComponent>(glm::vec3(0.0f, 2.0f, 2.0f), glm::vec3(0.0f), glm::vec3(1.0f));
	m_CameraTransform = std::make_shared<TransformComponent>(glm::vec3(5.0f, 0.5f, 5.0f), glm::vec3(0.0f), glm::vec3(1.0f)); // Forest
	m_CameraTransform->m_center = glm::vec3(0.0f,0.0f,0.0f);
	GraphicsMain::GetInstance()->m_MainCamera=m_CameraTransform;

	// サウンド
	std::string soundCode = {
		#include "Shader/Sound.frag"
	};

	// 背景色
	//GraphicsRenderer::GetInstance()->SetBackgroudColor(glm::vec4(glm::vec3(0.4f, 0.6f, 1.0f), 1.0));
	GraphicsRenderer::GetInstance()->SetBackgroudColor(glm::vec4(glm::vec3(0.75f), 1.0));
	
//#ifdef _DEBUG
	// デバッグ用グリッド
	m_GridPlane = std::make_shared<GameObject>(
		PrimitiveType::BOARD,
		RenderType::DefaultBuffer,
		RenderQueue::Geometry,
		RenderingSurfaceType::RASTERIZER,
		shaderlib::ShaderLib::Standard_vert,
		shaderlib::ShaderLib::GridPlane_frag
		);
	m_GridPlane->m_transform->m_rotation = glm::vec3(3.14159265f / 2.0f, 0.0, 0.0);
	m_GridPlane->m_transform->m_scale = glm::vec3(25.0f);
//#endif // _DEBUG

	// シーンオブジェクトの初期化
	//m_ProceduralCity = std::make_shared<myapp::ProceduralCity>();
	//m_SacredLake = std::make_shared<myapp::SacredLake>();
	m_Forest = std::make_shared<myapp::Forest>();
	//m_SSR_Test = std::make_shared<myapp::SSR_Test>();

	// ポストプロセスの設定
	//PostProcess::GetInstance()->m_UsePostProcess = true;
	//PostProcess::GetInstance()->m_UseBloom = true;
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
	/*m_CameraTransform->m_position = glm::vec3(
		glm::cos(GraphicsMain::GetInstance()->time * 0.001f) * 30.0f,
		10.0f,
		glm::sin(GraphicsMain::GetInstance()->time * 0.001f) * 30.0f
	);*/
	/*m_CameraTransform->m_position = glm::vec3(
		glm::cos(GraphicsMain::GetInstance()->time * 0.001f) * 2.0f,
		2.0f,
		glm::sin(GraphicsMain::GetInstance()->time * 0.001f) * 2.0f
	);*/
	/*m_CameraTransform->m_position = glm::vec3( // SacredLake
		glm::cos(GraphicsMain::GetInstance()->time * 0.001f*0.1f) * 30.0f,
		0.5f,
		glm::sin(GraphicsMain::GetInstance()->time * 0.001f * 0.1f) * 30.0f
	);*/
	
	m_CameraTransform->m_position = glm::vec3( // Forest
		glm::cos(GraphicsMain::GetInstance()->time * 0.001f*0.1f) * 5.0f,
		0.5f,
		glm::sin(GraphicsMain::GetInstance()->time * 0.001f * 0.1f) * 5.0f
	);

	m_Forest->Update();
}

void GenocideCronus::Draw() {
	//
	//m_ProceduralCity->Draw();
	//m_SacredLake->Draw();
	m_Forest->Draw();
	//m_SSR_Test->Draw();
}

void GenocideCronus::Timeline(CTimeline* timeline) {

}