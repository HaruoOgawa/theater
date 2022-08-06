#include "ProceduralCity.h"
#include "GraphicsEngine/Graphics/GraphicsRenderer.h"
#include "GraphicsEngine/Graphics/Mesh.h"
#include "GraphicsEngine/Graphics/Material.h"
#include "GraphicsEngine/Graphics/ShaderLib.h"
#include "GraphicsEngine/Component/TransformComponent.h"
#include "GraphicsEngine/Message/Console.h"
#include "GraphicsEngine/Object/GameObject.h"
#include "GraphicsEngine/Component/MeshRendererComponent.h"
#include "BillMeshGenerator.h"

namespace myapp {
	ProceduralCity::ProceduralCity()
	{
		Start();
	}

	void ProceduralCity::Start() 
	{	
		//#ifdef _DEBUG
		// デバッグ用グリッド
		m_GridPlane = std::make_shared<GameObject>(
			std::make_shared<TransformComponent>(),
			PrimitiveType::BOARD,
			RenderType::DefaultBuffer,
			RenderQueue::Geometry,
			RenderingSurfaceType::RASTERIZER,
			shaderlib::ShaderLib::Standard_vert,
			shaderlib::ShaderLib::GridPlane_frag
			);
		m_GridPlane->m_transform->m_rotation = glm::vec3(3.14159265f / 2.0f, 0.0, 0.0);
		m_GridPlane->m_transform->m_scale = glm::vec3(100.0f);
		//#endif // _DEBUG

		//
		m_BillMeshRenderer4Instanced = std::make_shared<MeshRendererComponent>(
			std::make_shared<TransformComponent>(),
			PrimitiveType::POINT,
			RenderingSurfaceType::RASTERIZER, 
			std::string(
				#include "../../Shader/ProceduralCity/ProceduralCity.vert"
			),
			std::string(
				#include "../../Shader/ProceduralCity/ProceduralCity.frag"
			),
			std::string(
				#include "../../Shader/ProceduralCity/ProceduralCity.geom"
			)
		);

		//// raymarching
		//std::string MandelboxShader = {
		//	#include "../../Shader/ProceduralCity/MandelboxSample.frag"
		//};

		/*m_Mandelbox = std::make_shared<MeshRendererComponent>(
			std::make_shared<TransformComponent>(),
			PrimitiveType::BOARD,
			RenderingSurfaceType::RAYMARCHING,
			shaderlib::ShaderLib::RaymarchingObject_vert,
			MandelboxShader
			);*/
	}

	void ProceduralCity::Update() 
	{
	}

	void ProceduralCity::Draw(bool IsRaymarching) {
		if (IsRaymarching)
		{
			//m_Mandelbox->Draw();
		}
		else
		{
			m_BillMeshRenderer4Instanced->Draw(GL_POINTS, true, 1024);
		}
	}
}