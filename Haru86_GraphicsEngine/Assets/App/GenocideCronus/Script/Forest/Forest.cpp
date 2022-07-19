#include "Forest.h"
#include "GraphicsEngine/Graphics/PostProcess.h"
#include "FlowerModel.h"

namespace myapp
{
	Forest::Forest():
		m_FlowerModel(nullptr)
	{
		Start();
	}

	void Forest::Start() {
		m_FlowerModel = std::make_shared<FlowerModel>();

		// Not Use SSR
		PostProcess::GetInstance()->m_UseSSR = false;
	}

	void Forest::Update() {
		m_FlowerModel->Update();
	}

	void Forest::Draw() {
		m_FlowerModel->Draw();
	}
}