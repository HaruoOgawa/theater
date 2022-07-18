#include "Forest.h"
#include "GraphicsEngine/Graphics/PostProcess.h"
#include "Flower.h"

namespace myapp
{
	Forest::Forest():
		m_Flower(nullptr)
	{
		Start();
	}

	void Forest::Start() {
		m_Flower = std::make_shared<Flower>();

		// Not Use SSR
		PostProcess::GetInstance()->m_UseSSR = false;
	}

	void Forest::Update() {
		m_Flower->Update();
	}

	void Forest::Draw() {
		m_Flower->Draw();
	}
}