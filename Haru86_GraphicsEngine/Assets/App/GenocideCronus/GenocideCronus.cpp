#include "GenocideCronus.h"

#include "GraphicsEngine/GraphicsMain/CTimeline.h"
#include "GraphicsEngine/Object/GameObject.h"
#include "GraphicsEngine/Graphics/ShaderLib.h"
#include "GraphicsEngine/Component/TransformComponent.h"
#include "GraphicsEngine/Graphics/GraphicsRenderer.h"
#include "GraphicsEngine/Message/Console.h"
#include "GraphicsEngine/Graphics/PostProcess.h"

#include "Assets/App/GenocideCronus/Script/ProceduralCity/ProceduralCity.h"
#include "Assets/App/GenocideCronus/Script/SacredLake/SacredLake.h"
#include "Assets/App/GenocideCronus/Script/Forest/Forest.h"
#include "Assets/App/GenocideCronus/Script/Mountain/Mountain.h"

#include "Assets/App/GenocideCronus/Script/SSR_Test/SSR_Test.h"

void GenocideCronus::Start() {
	//
	m_DebugSceneIndex = 0;

	// カメラ
	m_CameraTransform = std::make_shared<TransformComponent>();
	GraphicsMain::GetInstance()->m_MainCamera=m_CameraTransform;

	// シーンオブジェクトの初期化
	m_ProceduralCity = std::make_shared<myapp::ProceduralCity>();
	m_SacredLake = std::make_shared<myapp::SacredLake>();
	m_Forest = std::make_shared<myapp::Forest>();
	m_Mountain = std::make_shared<myapp::Mountain>();

	// ポストプロセスの設定
	PostProcess::GetInstance()->m_UsePostProcess = true;
	PostProcess::GetInstance()->m_UseBloom = true;
	PostProcess::GetInstance()->m_BloomThreshold = 1.0f;
	PostProcess::GetInstance()->m_BloomIntensity = 1.5f;

}

void GenocideCronus::Update() {
	//
	float time = GraphicsMain::GetInstance()->time * 0.001f * 0.25f;
	m_DebugSceneIndex = static_cast<int>(glm::floor(glm::mod(time, 3.99f)));

	m_DebugSceneIndex = 0;

	//
	if (m_DebugSceneIndex == 0) // City
	{
		// 描画設定
		PostProcess::GetInstance()->m_UseSSR = false;
		// 背景色
		if (m_DebugSceneIndex != 2)GraphicsRenderer::GetInstance()->SetBackgroudColor(glm::vec4(glm::vec3(0.75f), 1.0));

		// カメラ
		m_CameraTransform->m_position = glm::vec3(5.0f, 0.5f, 5.0f);
		m_CameraTransform->m_center = glm::vec3(0.0f, 5.0f, 0.0f);
	}
	else if (m_DebugSceneIndex == 1)
	{
		// 描画設定
		PostProcess::GetInstance()->m_UseSSR = false;
		// 背景色
		if (m_DebugSceneIndex != 2)GraphicsRenderer::GetInstance()->SetBackgroudColor(glm::vec4(glm::vec3(0.75f), 1.0));

		// カメラ
		float r = 10.0f;
		m_CameraTransform->m_position = glm::vec3( // Forest
			//glm::cos(GraphicsMain::GetInstance()->time * 0.001f*0.1f) * r,
			glm::cos(0.0f) * r,
			r,
			//glm::sin(GraphicsMain::GetInstance()->time * 0.001f * 0.1f) * r
			glm::sin(0.0f) * r
		);

		m_CameraTransform->m_center = glm::vec3(0.0f, 10.0f, 0.0f);

		// 処理更新
		m_Forest->Update();
	}
	else if (m_DebugSceneIndex == 2) // Lake
	{
		// 描画設定
		GraphicsRenderer::GetInstance()->SetBackgroudColor(glm::vec4(glm::vec3(0.4f, 0.6f, 1.0f), 1.0));

		// カメラ
		m_CameraTransform->m_center = glm::vec3(0.0f, 0.0f, 0.0f); // SacredLake

		m_CameraTransform->m_position = glm::vec3( // SacredLake
			glm::cos(GraphicsMain::GetInstance()->time * 0.001f * 0.1f) * 30.0f,
			0.5f,
			glm::sin(GraphicsMain::GetInstance()->time * 0.001f * 0.1f) * 30.0f
		);
	}
	else if (m_DebugSceneIndex == 3) // Mountain
	{
		// 描画設定
		PostProcess::GetInstance()->m_UseSSR = false;
		// 背景色
		if (m_DebugSceneIndex != 2)GraphicsRenderer::GetInstance()->SetBackgroudColor(glm::vec4(glm::vec3(0.75f), 1.0));

		// カメラ
		m_CameraTransform->m_position = glm::vec3(5.0f, 0.5f, 5.0f);
		m_CameraTransform->m_center = glm::vec3(0.0f, 10.0f, 0.0f);
	}
}

void GenocideCronus::Draw(bool IsRaymarching) {
	if (m_DebugSceneIndex == 0)m_ProceduralCity->Draw(IsRaymarching);
	if (m_DebugSceneIndex == 2)m_SacredLake->Draw(IsRaymarching);
	if (m_DebugSceneIndex == 1)m_Forest->Draw(IsRaymarching);
	if (m_DebugSceneIndex == 3)m_Mountain->Draw(IsRaymarching);
}

void GenocideCronus::Timeline(CTimeline* timeline) 
{
}