#include "Mountain.h"
#include "GraphicsEngine/Object/GameObject.h"
#include "GraphicsEngine/Component/MeshRendererComponent.h"
#include "GraphicsEngine/Component/TransformComponent.h"
#include "GraphicsEngine/GraphicsMain/GraphicsMain.h"

namespace myapp {
	Mountain::Mountain()
	{
		m_MoutainObj = std::make_shared<MeshRendererComponent>(
			std::make_shared<TransformComponent>(),
			PrimitiveType::BOARD,
			RenderingSurfaceType::RAYMARCHING,
			shaderlib::ShaderLib::StandardRenderBoard_vert, 
			std::string(
				#include "../../Shader/Mountain/Mountain.frag"
			)
		);

		m_MoutainObj->useZTest = false;
	}

	void Mountain::Update()
	{
	}

	void Mountain::Draw(bool IsRaymarching)
	{
		if (IsRaymarching)
		{
			m_MoutainObj->Draw();
		}
	}

	void Mountain::UpdateTimeline(float LocalTime)
	{
		if (GraphicsMain::GetInstance()->GetAppSceneIndex() == 3)
		{
			
			// ƒJƒƒ‰ƒ[ƒN
			float CameraTimeModeRate = 5.0f, NumOfCamera = 5.0f;
			float CameraworkTime = glm::mod(LocalTime, CameraTimeModeRate * NumOfCamera);

			if (CameraworkTime >= 0.0f && CameraworkTime < CameraTimeModeRate * 1.0f)
			{
				GraphicsMain::GetInstance()->m_MainCamera->m_position = glm::vec3(20.0f * glm::cos(LocalTime), 2.0f, 20.0f * glm::sin(LocalTime))*200.0f;
				GraphicsMain::GetInstance()->m_MainCamera->m_center = glm::vec3(0.0f, 10.0f, 0.0f);
			}
			else if (CameraworkTime >= CameraTimeModeRate * 1.0f && CameraworkTime < CameraTimeModeRate * 2.0f)
			{
				GraphicsMain::GetInstance()->m_MainCamera->m_position = glm::vec3(2.0f * glm::cos(-3.14f / 2.0), 1.0f, 2.0f * glm::sin(-3.14f / 2.0)) * 200.0f;
				GraphicsMain::GetInstance()->m_MainCamera->m_center = glm::vec3(0.0f, 100.0f, 0.0f);
			}
			else if (CameraworkTime >= CameraTimeModeRate * 2.0f && CameraworkTime < CameraTimeModeRate * 3.0f)
			{
				GraphicsMain::GetInstance()->m_MainCamera->m_position = glm::vec3(2.5f, 2.5f, 0.0f) * 200.0f;
				GraphicsMain::GetInstance()->m_MainCamera->m_center = glm::vec3(0.0f, 100.0f, 0.0f);
			}
			else if (CameraworkTime >= CameraTimeModeRate * 3.0f && CameraworkTime < CameraTimeModeRate * 4.0f)
			{
				GraphicsMain::GetInstance()->m_MainCamera->m_position = glm::vec3(2.5f, 0.5f, 2.5f) * 200.0f;
				GraphicsMain::GetInstance()->m_MainCamera->m_center = glm::vec3(0.0f, 100.0f, 0.0f);
			}
			else if (CameraworkTime >= CameraTimeModeRate * 4.0f && CameraworkTime < CameraTimeModeRate * 5.0f)
			{
				GraphicsMain::GetInstance()->m_MainCamera->m_position = glm::vec3(0.1f, 10.0f, 0.0f) * 200.0f;
				GraphicsMain::GetInstance()->m_MainCamera->m_center = glm::vec3(0.0f, 100.0f, 0.0f);
			}
		}
	}
}