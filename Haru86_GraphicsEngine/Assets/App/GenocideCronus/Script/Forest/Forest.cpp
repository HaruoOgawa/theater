#include "Forest.h"
#include "GraphicsEngine/Graphics/PostProcess.h"
#include "FlowerModel.h"
#include "LTreeModel.h"
#include "GraphicsEngine/Object/GameObject.h"
#include "GraphicsEngine/Component/MeshRendererComponent.h"
#include "GraphicsEngine/Graphics/ShaderLib.h"
#include "GraphicsEngine/Component/TransformComponent.h"

namespace myapp
{
	Forest::Forest():
		m_FlowerModel(nullptr),
		m_LTreeModel(nullptr),
		m_EnergyBall(nullptr),
		m_Ground(nullptr),
		m_AroundMountain(nullptr)
	{
		Start();
	}

	void Forest::Start() {
		// 花
		m_FlowerModel = std::make_shared<FlowerModel>();
		
		// 木
		m_LTreeModel = std::make_shared<LTreeModel>();

		// エナジーボール
		std::string EnergyBall_frag = {
			#include "../../Shader/Forest/EnergyBall.frag"
		};
		m_EnergyBall = std::make_shared<MeshRendererComponent>(
			std::make_shared<TransformComponent>(),
			PrimitiveType::BOARD,
			RenderingSurfaceType::RAYMARCHING,shaderlib::ShaderLib::StandardRenderBoard_vert, EnergyBall_frag
		);
		m_EnergyBall->useZTest = false;
		m_EnergyBall->useAlphaTest = true;

		// 地面
		std::string MountainGround_frag = {
			#include "../../Shader/Forest/MountainGround.frag"
		};

		m_Ground = std::make_shared<MeshRendererComponent>(
			std::make_shared<TransformComponent>(),
			PrimitiveType::BOARD,
			RenderingSurfaceType::RASTERIZER, shaderlib::ShaderLib::Standard_vert, MountainGround_frag);

		m_Ground->m_transform->m_rotation = glm::vec3(-3.14f / 2.0f, 0.0f, 0.0f);
		m_Ground->m_transform->m_scale = glm::vec3(500.0f);
		m_Ground->m_calllback = [this]() 
		{
			m_Ground->m_material->SetIntUniform("_UseColor", 1);
			m_Ground->m_material->SetVec4Uniform("_Color", glm::vec4(0.5f*glm::vec3(227.0 / 255.0, 206.0 / 255.0, 164.0 / 255.0), 1.0f));
		};

		// Mountain(Scene4のものを背景として流用)
		std::string MountainAround_frag = {
			#include "../../Shader/Forest/MountainAround.frag"
		};
		m_AroundMountain = std::make_shared<MeshRendererComponent>(
			std::make_shared<TransformComponent>(),
			PrimitiveType::BOARD,
			RenderingSurfaceType::RAYMARCHING, shaderlib::ShaderLib::StandardRenderBoard_vert, MountainAround_frag);

		m_AroundMountain->useAlphaTest = false;
		m_AroundMountain->useZTest = false;

		// Not Use SSR
		PostProcess::GetInstance()->m_UseSSR = false;
	}

	void Forest::Update() {
		m_FlowerModel->Update();
		m_LTreeModel->Update();
	}

	void Forest::Draw(bool IsRaymarching, int SceneIndex, int LinearInstanceRate) {
		//
		bool UseFade = false;
		
		//
		if (SceneIndex == 1)
		{
			m_LTreeModel->SetIsStreetLineTree(true);
			if (!IsRaymarching)
			{
				UseFade = true;

				m_LTreeModel->m_TreeRenderer->m_transform->m_scale = glm::vec3(0.05f);
				int numOfTree = static_cast<int>(glm::pow(2.0f, LinearInstanceRate));
				m_LTreeModel->Draw(numOfTree, LinearInstanceRate, UseFade);
			}
		}
		else if(SceneIndex == 2)
		{
			m_LTreeModel->SetIsStreetLineTree(false);
			if (IsRaymarching)
			{
				//m_AroundMountain->Draw();
				m_EnergyBall->Draw();
			}
			else
			{
				m_LTreeModel->m_TreeRenderer->m_transform->m_scale = glm::vec3(1.0f);
				m_Ground->Draw();
				m_FlowerModel->Draw();
				m_LTreeModel->Draw();
			}
		}
	}

	void Forest::UpdateTimeline(float LocalTime)
	{

	}
}