#include "Forest.h"
#include "GraphicsEngine/Graphics/PostProcess.h"

namespace myapp
{
	Forest::Forest() {

	}

	void Forest::Start() {
		PostProcess::GetInstance()->m_UseSSR = false;
	}

	void Forest::Update() {

	}

	void Forest::Draw() {

	}
}