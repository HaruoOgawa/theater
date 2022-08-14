#include "Mountain.h"
#include "GraphicsEngine/Object/GameObject.h"
#include "GraphicsEngine/Component/MeshRendererComponent.h"
#include "GraphicsEngine/Component/TransformComponent.h"

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
}