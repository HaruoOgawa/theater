#include "Mountain.h"
#include "GraphicsEngine/Object/GameObject.h"
#include "GraphicsEngine/Component/MeshRendererComponent.h"
#include "GraphicsEngine/Component/TransformComponent.h"

namespace myapp {
	Mountain::Mountain()
	{
		std::string Mountain_frag = {
			#include "../../Shader/Mountain/Mountain.frag"
		};

		m_MoutainObj = std::make_shared<MeshRendererComponent>(
			std::make_shared<TransformComponent>(),
			PrimitiveType::BOARD,
			RenderingSurfaceType::RAYMARCHING,
			shaderlib::ShaderLib::StandardRenderBoard_vert, Mountain_frag);

		m_MoutainObj->useZTest = false;
	}

	void Mountain::Update()
	{

	}

	void Mountain::Draw()
	{
		m_MoutainObj->Draw();
	}
}