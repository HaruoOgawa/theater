#include "GenocideCronus.h"

#include "GraphicsEngine/GraphicsMain/CTimeline.h"
#include "GraphicsEngine/Object/GameObject.h"
#include "GraphicsEngine/Graphics/ShaderLib.h"
#include "GraphicsEngine/Component/TransformComponent.h"
#include "GraphicsEngine/Component/MeshRendererComponent.h"
#include "GraphicsEngine/Graphics/GraphicsRenderer.h"
#include "GraphicsEngine/Message/Console.h"
#include "GraphicsEngine/Graphics/PostProcess.h"
#include "Assets/App/GenocideCronus/Script/ProceduralCity/ProceduralCity.h"
#include "Assets/App/GenocideCronus/Script/SacredLake/SacredLake.h"
#include "Assets/App/GenocideCronus/Script/Forest/Forest.h"
#include "Assets/App/GenocideCronus/Script/Mountain/Mountain.h"
#include "GraphicsEngine/Sound/SoundPlayer.h"

GenocideCronus::GenocideCronus():
	m_CameraTransform(nullptr),
	m_GridPlane(nullptr),
	m_SceneIndex(0),
	m_ProceduralCity(nullptr),
	m_SacredLake(nullptr),
	m_Forest(nullptr),
	m_Mountain(nullptr),
	m_LocalTime(0.0f),
	m_LocalReverseTime(0.0f),
	m_IsReverseTime(false),
	m_DebugTimeLock(false)
{
}

void GenocideCronus::Start() {
#ifdef _DEBUG
	// 時間のオフセット
	GraphicsMain::GetInstance()->m_SecondsTimeOffset = 160.0f;// シーンを飛ばすためのオフセット

	// デバッグ用
	/*{
		m_DebugTimeLock = true;
		if (m_DebugTimeLock)m_LocalTime = GraphicsMain::GetInstance()->m_SecondsTimeOffset;
	}*/
#endif

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
	PostProcess::GetInstance()->m_UseBloom = false;
	PostProcess::GetInstance()->m_BloomThreshold = 0.0f; // 1
	PostProcess::GetInstance()->m_BloomIntensity = 0.0f; // 4

}

void GenocideCronus::Update() {
	//
	if (m_LocalTime >= 0.0f && m_LocalTime < 70.0f) // シーン1(City)
	{
		m_SceneIndex = 0;
	}
	else if (m_LocalTime >= 70.0f && m_LocalTime < 85.0f) // シーン2(City&Forest)
	{
		m_SceneIndex = 1;
	}
	else if (m_LocalTime >= 85.0f && m_LocalTime < 116.0f) // シーン3(Forest)
	{
		m_SceneIndex = 2;
	}
	else if (m_LocalTime >= 116.0f && m_LocalTime < 150.0f) //  シーン4(Mountain)
	{
		m_SceneIndex = 3;
	}
	else if (m_LocalTime >= 150.0f && m_LocalTime < 161.0f) // シーン5(Lake)
	{
		m_SceneIndex = 4;
	}
	else if (m_LocalTime >= 161.0f && m_LocalTime < 177.0f) // シーン6(巻き戻し)
	{
		m_SceneIndex = 5;
		PostProcess::GetInstance()->m_LatePostProcesCallBack = []() {
			PostProcess::GetInstance()->m_LateMeshRenderer->m_material->SetIntUniform("_UseRewinding", 1);
		};

		m_IsReverseTime = true;
		m_LocalReverseTime = m_LocalTime;
	}
	else if (m_LocalTime >= 177.0f && m_LocalTime < 264.0f) // シーン7(End of City)
	{
		m_SceneIndex = 6;
	}
	else if(m_LocalTime >= 264.0f)// デモ終了
	{
		m_SceneIndex = 7;
		GraphicsMain::GetInstance()->isRunning = false;
	}

	//m_SceneIndex = 3;

	// シーン制御
	if (m_SceneIndex == 0 || m_SceneIndex == 1 || m_SceneIndex == 6) // City
	{
		// 描画設定
		PostProcess::GetInstance()->m_UseSSR = false;
		// 背景色
		GraphicsRenderer::GetInstance()->SetBackgroudColor(glm::vec4(glm::vec3(0.4f, 0.6f, 1.0f), 1.0));
		// ライトポジション
		GraphicsMain::GetInstance()->m_GroabalLightPosition->m_position = glm::vec3(
			10.0f * glm::cos(-3.14f / 2.0),
			//10.0f * glm::cos(GraphicsMain::GetInstance()->m_SecondsTime),
			10.0f,
			10.0f * glm::sin(-3.14f / 2.0)
			//10.0f * glm::sin(GraphicsMain::GetInstance()->m_SecondsTime)
		);

		// 更新処理
		m_ProceduralCity->Update();
	}
	else if (m_SceneIndex == 2) // Forest
	{
		// 描画設定
		PostProcess::GetInstance()->m_UseSSR = false;
		// 背景色
		//GraphicsRenderer::GetInstance()->SetBackgroudColor(glm::vec4(glm::vec3(0.4f, 0.6f, 1.0f), 1.0));
		GraphicsRenderer::GetInstance()->SetBackgroudColor(glm::vec4(glm::vec3(0.75f), 1.0));

		// ライトポジション
		GraphicsMain::GetInstance()->m_GroabalLightPosition->m_position = glm::vec3(
			0.0f,
			5.0f,
			0.0f
		);

		// 更新処理
		m_Forest->Update();
	}
	else if (m_SceneIndex == 3) // Mountain
	{
		// 描画設定
		PostProcess::GetInstance()->m_UseSSR = false;
		// 背景色
		GraphicsRenderer::GetInstance()->SetBackgroudColor(glm::vec4(glm::vec3(0.75f), 1.0));
	}
	else if (m_SceneIndex == 4) // Lake
	{
		// 描画設定
		GraphicsRenderer::GetInstance()->SetBackgroudColor(glm::vec4(glm::vec3(0.4f, 0.6f, 1.0f), 1.0));

		// カメラ
		m_CameraTransform->m_center = glm::vec3(0.0f, 0.0f, 0.0f); // SacredLake

		m_CameraTransform->m_position = glm::vec3( // SacredLake
			//glm::cos(0.0f) * 30.0f,
			glm::cos(GraphicsMain::GetInstance()->m_SecondsTime * 0.1f) * 30.0f,
			0.5f,
			//glm::sin(0.0f) * 30.0f
			glm::sin(GraphicsMain::GetInstance()->m_SecondsTime * 0.1f) * 30.0f
		);

		// 更新処理
		m_SacredLake->Update();
	}
}

void GenocideCronus::UpdateTimeline()
{
	// 時間

	if (!m_DebugTimeLock) {
		if (m_IsReverseTime) // 巻き戻し
		{
			// 巻き戻しの速さを計算する
			float ReverseTimeRate = (161.0f - 70.0f) / 16.0f;

			//
			m_LocalReverseTime -= GraphicsMain::GetInstance()->m_DeltaTime * ReverseTimeRate;
			m_LocalTime = m_LocalReverseTime;

			if (m_LocalReverseTime <= 70.0f && GraphicsMain::GetInstance()->m_SecondsTime >= 177.0f)
			{
				m_IsReverseTime = false;
				m_LocalTime = GraphicsMain::GetInstance()->m_SecondsTime;
			}
		}
		else // 通常の再生
		{
			m_LocalTime = GraphicsMain::GetInstance()->m_SecondsTime;
		}
	}

#ifdef _DEBUG
	Console::Log("m_LocalTime: %f\n", m_LocalTime);
#endif // _DEBUG


	// タイムラインを更新
	m_ProceduralCity->UpdateTimeline(m_LocalTime, m_IsReverseTime);
	m_Forest->UpdateTimeline(m_LocalTime, m_IsReverseTime);
	m_Mountain->UpdateTimeline(m_LocalTime, m_IsReverseTime);
	m_SacredLake->UpdateTimeline(m_LocalTime, m_IsReverseTime);
}

void GenocideCronus::Draw(bool IsRaymarching) {
	//
	int LinearInstanceRate = 0;
	if (m_LocalTime >= 70.0f && m_LocalTime < 85.0f) // シーン2(City&Forest)
	{
		LinearInstanceRate = static_cast<int>(glm::clamp((m_LocalTime - 70.0f) / 10.0f, 0.0f, 1.0f) * 10.0f);
		LinearInstanceRate = glm::min(5, LinearInstanceRate);
	}

	//
	if (m_SceneIndex == 0 || m_SceneIndex == 1 || m_SceneIndex == 6)m_ProceduralCity->Draw(IsRaymarching, LinearInstanceRate);
	if (m_SceneIndex == 2 || m_SceneIndex == 1)m_Forest->Draw(IsRaymarching, m_SceneIndex, glm::min(10, LinearInstanceRate + 4), m_IsReverseTime);
	if (m_SceneIndex == 3)m_Mountain->Draw(IsRaymarching);
	if (m_SceneIndex == 4)m_SacredLake->Draw(IsRaymarching);
}

void GenocideCronus::Timeline(CTimeline* timeline) 
{
}