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
#include "GraphicsEngine/Graphics/ReflectionProbe.h"

namespace myapp {
	ProceduralCity::ProceduralCity():
		m_BillRPProgress(BillRPProgress::Initialize),
		m_BillRP(nullptr),
		m_RPDrawCount(0),
		m_Street(nullptr)
	{
		Start();
	}

	void ProceduralCity::Start() 
	{	
		#ifdef _DEBUG
		// デバッグ用グリッド
		/*m_GridPlane = std::make_shared<GameObject>(
			std::make_shared<TransformComponent>(),
			PrimitiveType::BOARD,
			RenderType::DefaultBuffer,
			RenderQueue::Geometry,
			RenderingSurfaceType::RASTERIZER,
			shaderlib::ShaderLib::Standard_vert,
			shaderlib::ShaderLib::GridPlane_frag
			);
		m_GridPlane->m_transform->m_rotation = glm::vec3(3.14159265f / 2.0f, 0.0, 0.0);
		m_GridPlane->m_transform->m_scale = glm::vec3(100.0f);*/

		// デバッグ用スフィア
		/*m_DebugSphere = std::make_shared<MeshRendererComponent>(
			std::make_shared<TransformComponent>(),
			PrimitiveType::SPHERE,
			RenderingSurfaceType::RASTERIZER,
			shaderlib::ShaderLib::Standard_vert,
			shaderlib::ShaderLib::Standard_frag
		);
		m_DebugSphere->m_transform->m_position = glm::vec3(0.0, 1.0f, 0.0f);
		m_DebugSphere->m_transform->m_scale = glm::vec3(0.5f);*/

		#endif // _DEBUG

		//
		{
			// VertexDataを構築
			std::vector<std::vector<float>> VertexData; std::vector<int> Dimention; std::vector<unsigned short> Indices;
			BillMeshGenerator Generator;
			Generator.Generate(VertexData, Dimention, Indices);
			
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
				),
				std::string(
					#include "../../Shader/ProceduralCity/Bill.geom"
				),
				std::string(
					#include "../../Shader/ProceduralCity/Bill.tesc"
				),
				std::string(
					#include "../../Shader/ProceduralCity/Bill.tese"
				)
			);
		}
		
		// ストリート
		{
			/*std::vector<std::vector<float>> VertexData; std::vector<int> Dimention; std::vector<unsigned short> Indices;
			BillMeshGenerator Generator;
			Generator.GenerateStreet(VertexData, Dimention, Indices);*/

			m_Street = std::make_shared<MeshRendererComponent>(
					std::make_shared<TransformComponent>(),
				PrimitiveType::BOARD,
				RenderingSurfaceType::RASTERIZER,
				//VertexData, Dimention, Indices,
				std::string(
					#include "../../Shader/ProceduralCity/Street.vert"
				),
				std::string(
					#include "../../Shader/ProceduralCity/Street.frag"
				),
				std::string(
					#include "../../Shader/ProceduralCity/Street.geom"
				),
				std::string(
					#include "../../Shader/ProceduralCity/Street.tesc"
				),
				std::string(
					#include "../../Shader/ProceduralCity/Street.tese"
				)
			);

			m_Street->m_transform->m_scale = glm::vec3(100.0f);
			m_Street->m_transform->m_rotation = glm::vec3(3.14f / 2.0f, 0.0f, 0.0f);
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
		/*std::string MandelboxShader = {
			#include "../../Shader/ProceduralCity/MandelboxSample.frag"
		};*/

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
		/*GraphicsMain::GetInstance()->m_MainCamera->m_position = glm::vec3(
			3.0f * glm::cos(GraphicsMain::GetInstance()->time * 0.001f),
			3.0f,
			3.0f*glm::sin(GraphicsMain::GetInstance()->time*0.001f)
		);
		GraphicsMain::GetInstance()->m_MainCamera->m_center = glm::vec3(0.0f, 2.0f, 0.0f);*/

		// デバッグ用カメラ
		float radius = 2.0f;
		GraphicsMain::GetInstance()->m_MainCamera->m_position = glm::vec3(
			radius * glm::cos(-3.14f/2.0),
			//radius * glm::cos(GraphicsMain::GetInstance()->time * 0.001f),
			0.5f,
			radius * glm::sin(-3.14f / 2.0)
			//radius * glm::sin(GraphicsMain::GetInstance()->time * 0.001f)
		);
		GraphicsMain::GetInstance()->m_MainCamera->m_center = glm::vec3(0.0f, 0.5f, 0.0f);

		// デバッグ用ライト移動
		GraphicsMain::GetInstance()->m_GroabalLightPosition->m_position = glm::vec3(
			10.0f * glm::cos(-3.14f / 2.0),
			//10.0f * glm::cos(GraphicsMain::GetInstance()->time * 0.001f),
			10.0f,
			10.0f * glm::sin(-3.14f / 2.0)
			//10.0f * glm::sin(GraphicsMain::GetInstance()->time * 0.001f)
		);
	}

	void ProceduralCity::Draw(bool IsRaymarching) {
		if (IsRaymarching)
		{
			//m_Mandelbox->Draw();
		}
		else
		{
//#ifdef _DEBUG
//			if (m_BillRP)
//			{
//				m_DebugSphere->Draw(GL_TRIANGLES, false, 0, [this]() {
//					m_BillRP->m_CubeTex->SetActive(GL_TEXTURE1,GL_TEXTURE_CUBE_MAP);
//					m_DebugSphere->m_material->SetIntUniform("_UseMainTex", 1);
//					m_DebugSphere->m_material->SetTexUniform("_MainTex", 1);
//					});
//				m_BillRP->m_CubeTex->SetEnactive(GL_TEXTURE1, GL_TEXTURE_CUBE_MAP);
//			}
//#endif
			// ビル
			if (m_BillRP)
			{
				//m_ProceduralBillRenderer->Draw(GL_PATCHES);
				m_ProceduralBillRenderer->Draw(GL_PATCHES, true, 1024, [this]() {
					m_BillRP->m_CubeTex->SetActive(GL_TEXTURE1, GL_TEXTURE_CUBE_MAP);
					m_ProceduralBillRenderer->m_material->SetTexUniform("_BillRP", 1);
					});
				m_BillRP->m_CubeTex->SetEnactive(GL_TEXTURE1, GL_TEXTURE_CUBE_MAP);
			}
			
			// ストリート
			if (m_Street)
			{
				//m_Street->Draw();
				m_Street->Draw(GL_PATCHES);
			}

			//m_BillMeshRenderer4Instanced->Draw(GL_POINTS, true, 1024);

			// RPの処理
			// RPを3つぐらい作って縦方向にOffsetする
			// Shaderでは座標のZ値をmodしてどのRPを使うか決める
			// こうすればある程度まばらに見えるはず
			switch (m_BillRPProgress)
			{
			case myapp::BillRPProgress::Initialize:
				//m_BillRP = std::make_shared<ReflectionProbe>(GraphicsMain::GetInstance()->m_MainCamera);
				m_BillRP = std::make_shared<ReflectionProbe>(glm::vec3(0.0f,2.5f ,0.0f),0.001f);
				GraphicsMain::GetInstance()->m_ReflectionProbeList.push_back(m_BillRP);

				m_BillRPProgress = BillRPProgress::Draw;
				break;
			case myapp::BillRPProgress::Draw:
				m_RPDrawCount++;

				if(m_RPDrawCount>2)m_BillRPProgress = BillRPProgress::Separation;
				break;
			case myapp::BillRPProgress::Separation:
			{
				auto& RPList = GraphicsMain::GetInstance()->m_ReflectionProbeList;
				auto Item = std::find(RPList.begin(), RPList.end(), m_BillRP);
				if (Item != RPList.end())
				{
					RPList.erase(RPList.begin() + std::distance(RPList.begin(), Item));
				}

				m_BillRPProgress = BillRPProgress::None;
				break;
			}
			case myapp::BillRPProgress::None:
				break;
			default:
				break;
			}
		}
	}
}