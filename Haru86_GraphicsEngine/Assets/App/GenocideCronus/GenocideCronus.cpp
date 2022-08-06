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
#include "Assets/App/GenocideCronus/Script/Forest/Forest.h"
#include "Assets/App/GenocideCronus/Script/Mountain/Mountain.h"

#include "Assets/App/GenocideCronus/Script/SSR_Test.h"

void GenocideCronus::Start() {
	
	m_DebugSceneIndex = 0;

	// カメラ
	if(m_DebugSceneIndex==2)m_CameraTransform = std::make_shared<TransformComponent>(glm::vec3(0.0f, 0.5f, 30.0f), glm::vec3(0.0f), glm::vec3(1.0f)); // SacredLake
	if (m_DebugSceneIndex == 1)m_CameraTransform = std::make_shared<TransformComponent>(glm::vec3(5.0f, 0.5f, 5.0f), glm::vec3(0.0f), glm::vec3(1.0f)); // Forest
	if (m_DebugSceneIndex == 0 || m_DebugSceneIndex == 3)m_CameraTransform = std::make_shared<TransformComponent>(glm::vec3(5.0f, 0.5f, 5.0f), glm::vec3(0.0f), glm::vec3(1.0f)); // City/Forest/Mountain
	
	if (m_DebugSceneIndex == 2)m_CameraTransform->m_center = glm::vec3(0.0f,0.0f,0.0f); // SacredLake
	if (m_DebugSceneIndex == 0)m_CameraTransform->m_center = glm::vec3(0.0f,5.0f,0.0f); // City
	if (m_DebugSceneIndex == 1 || m_DebugSceneIndex == 3) m_CameraTransform->m_center = glm::vec3(0.0f,10.0f,0.0f); // Forest/Mountain

	GraphicsMain::GetInstance()->m_MainCamera=m_CameraTransform;

	// 背景色
	if (m_DebugSceneIndex == 2)GraphicsRenderer::GetInstance()->SetBackgroudColor(glm::vec4(glm::vec3(0.4f, 0.6f, 1.0f), 1.0));
	if (m_DebugSceneIndex != 2)GraphicsRenderer::GetInstance()->SetBackgroudColor(glm::vec4(glm::vec3(0.75f), 1.0));
	
//#ifdef _DEBUG
	/*// デバッグ用グリッド
	m_GridPlane = std::make_shared<GameObject>(
		PrimitiveType::BOARD,
		RenderType::DefaultBuffer,
		RenderQueue::Geometry,
		RenderingSurfaceType::RASTERIZER,
		shaderlib::ShaderLib::Standard_vert,
		shaderlib::ShaderLib::GridPlane_frag
		);
	m_GridPlane->m_transform->m_rotation = glm::vec3(3.14159265f / 2.0f, 0.0, 0.0);
	m_GridPlane->m_transform->m_scale = glm::vec3(100.0f);*/
//#endif // _DEBUG

	// シーンオブジェクトの初期化
	/*if (m_DebugSceneIndex == 0)*/m_ProceduralCity = std::make_shared<myapp::ProceduralCity>();
	/*if (m_DebugSceneIndex == 2)*/m_SacredLake = std::make_shared<myapp::SacredLake>();
	/*if (m_DebugSceneIndex == 1)*/m_Forest = std::make_shared<myapp::Forest>();
	/*if (m_DebugSceneIndex == 3)*/m_Mountain = std::make_shared<myapp::Mountain>();

	// ポストプロセスの設定
	PostProcess::GetInstance()->m_UsePostProcess = true;
	PostProcess::GetInstance()->m_UseBloom = true;
	PostProcess::GetInstance()->m_BloomThreshold = 1.0f;
	PostProcess::GetInstance()->m_BloomIntensity = 1.5f;

}

void GenocideCronus::Update() {
	if(m_DebugSceneIndex!=2)PostProcess::GetInstance()->m_UseSSR = false;

	// カメラ
	if (m_DebugSceneIndex == 2)
	{
		m_CameraTransform->m_position = glm::vec3( // SacredLake
			glm::cos(GraphicsMain::GetInstance()->time * 0.001f * 0.1f) * 30.0f,
			0.5f,
			glm::sin(GraphicsMain::GetInstance()->time * 0.001f * 0.1f) * 30.0f
		);
	}
	
	if (m_DebugSceneIndex == 1)
	{
		float r = 10.0f;
		m_CameraTransform->m_position = glm::vec3( // Forest
			//glm::cos(GraphicsMain::GetInstance()->time * 0.001f*0.1f) * r,
			glm::cos(0.0f) * r,
			r,
			//glm::sin(GraphicsMain::GetInstance()->time * 0.001f * 0.1f) * r
			glm::sin(0.0f) * r
		);

		m_Forest->Update();
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