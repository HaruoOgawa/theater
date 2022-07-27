#include "Forest.h"
#include "GraphicsEngine/Graphics/PostProcess.h"
#include "FlowerModel.h"
#include "LTreeModel.h"
#include "GraphicsEngine/Object/GameObject.h"
#include "GraphicsEngine/Component/MeshRendererComponent.h"
#include "GraphicsEngine/Graphics/ShaderLib.h"

namespace myapp
{
	Forest::Forest():
		m_FlowerModel(nullptr),
		m_LTreeModel(nullptr),
		m_EnergyBall(nullptr)
	{
		Start();
	}

	void Forest::Start() {
		m_FlowerModel = std::make_shared<FlowerModel>();
		m_LTreeModel = std::make_shared<LTreeModel>();

		std::string EnergyBall_frag = {
			#include "../../Shader/Forest/EnergyBall.frag"
		};
		m_EnergyBall = std::make_shared<GameObject>(PrimitiveType::BOARD, RenderType::DefaultBuffer, RenderQueue::Geometry,
			RenderingSurfaceType::RAYMARCHING,shaderlib::ShaderLib::StandardRenderBoard_vert, EnergyBall_frag
		);
		m_EnergyBall->meshComp->useZTest = false;
		m_EnergyBall->meshComp->useAlphaTest = true;

		// Not Use SSR
		PostProcess::GetInstance()->m_UseSSR = false;
	}

	void Forest::Update() {
		m_FlowerModel->Update();
		m_LTreeModel->Update();
	}

	void Forest::Draw() {
		m_FlowerModel->Draw();
		m_LTreeModel->Draw();
	}
}