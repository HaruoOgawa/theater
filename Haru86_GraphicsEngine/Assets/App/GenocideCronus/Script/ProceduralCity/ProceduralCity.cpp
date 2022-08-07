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
		{
			// VertexDataを構築
			std::vector<std::vector<float>> VertexData; std::vector<int> Dimention; std::vector<unsigned short> Indices;
			BillMeshGenerator Generator;
			Generator.Generate(VertexData, Dimention, Indices);
			
			/*for (const auto& Val : VertexData[0]){ Console::Log("Vertices Val: %f\n", Val); }
			for (const auto& Val : VertexData[1]) { Console::Log("Normal Val: %f\n", Val); }
			for (const auto& Val : Dimention) { Console::Log("Dimention Val: %d\n", Val); }
			for (const auto& Val : Indices) { Console::Log("Indices Val: %d\n", Val); }*/

			// ビルのレンダラーを構築
			m_ProceduralBillRenderer = std::make_shared<MeshRendererComponent>(
				std::make_shared<TransformComponent>(),
				RenderingSurfaceType::RASTERIZER,
				VertexData,Dimention,Indices,
				std::string(
					#include "../../Shader/ProceduralCity/Bill.vert"
				),
				std::string(
					#include "../../Shader/ProceduralCity/Bill.frag"
				)
			);
		}
		

		//
		/*m_BillMeshRenderer4Instanced = std::make_shared<MeshRendererComponent>(
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
		);*/

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
		// デバッグ用カメラ
		//GraphicsMain::GetInstance()->m_MainCamera->m_position = glm::vec3(10.0f, 1.0f, 10.0f);
		GraphicsMain::GetInstance()->m_MainCamera->m_position = glm::vec3(
			3.0f * glm::cos(GraphicsMain::GetInstance()->time * 0.001f),
			3.0f,
			3.0f*glm::sin(GraphicsMain::GetInstance()->time*0.001f)
		);
		GraphicsMain::GetInstance()->m_MainCamera->m_center = glm::vec3(0.0f, 2.0f, 0.0f);
	}

	void ProceduralCity::Draw(bool IsRaymarching) {
		if (IsRaymarching)
		{
			//m_Mandelbox->Draw();
		}
		else
		{
			m_ProceduralBillRenderer->Draw();
			//m_BillMeshRenderer4Instanced->Draw(GL_POINTS, true, 1024);
		}
	}
}