#include "Forest.h"
#include "GraphicsEngine/Graphics/PostProcess.h"
#include "FlowerModel.h"
#include "LTreeModel.h"

namespace myapp
{
	Forest::Forest():
		m_FlowerModel(nullptr),
		m_LTreeModel(nullptr)
	{
		Start();
	}

	void Forest::Start() {
		m_FlowerModel = std::make_shared<FlowerModel>();
		m_LTreeModel = std::make_shared<LTreeModel>();

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