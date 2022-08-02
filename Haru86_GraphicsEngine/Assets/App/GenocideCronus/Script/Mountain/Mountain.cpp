#include "Mountain.h"
#include "GraphicsEngine/Object/GameObject.h"
#include "GraphicsEngine/Component/MeshRendererComponent.h"

namespace myapp {
	Mountain::Mountain()
	{
		std::string Mountain_frag = {
			#include "../../Shader/Mountain/Mountain.frag"
		};

		m_MoutainObj = std::make_shared<GameObject>(PrimitiveType::BOARD, RenderType::DefaultBuffer, RenderQueue::Geometry,
			RenderingSurfaceType::RAYMARCHING, shaderlib::ShaderLib::StandardRenderBoard_vert, Mountain_frag);

		m_MoutainObj->meshComp->useZTest = false;
	}
}