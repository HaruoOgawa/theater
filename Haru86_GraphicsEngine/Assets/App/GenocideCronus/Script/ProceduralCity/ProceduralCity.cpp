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
		m_BillRP(std::make_shared<ReflectionProbe>(glm::vec3(0.0f, 2.5f, 0.0f), 0.001f)),
		m_Mandelbox(nullptr),
		m_ProceduralBillRenderer(nullptr),
		m_Street(nullptr),
		m_StreeLamp(nullptr),
		m_CylinderBill(nullptr),
		m_XSideWarkVec(glm::vec3(0.0f))
	{
		Start();
	}

	void ProceduralCity::Start() 
	{	
		// ビル
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
		
		// 円柱ビル
		{
			// VertexDataを構築
			std::vector<std::vector<float>> VertexData; std::vector<int> Dimention; std::vector<unsigned short> Indices;
			BillMeshGenerator Generator;
			Generator.GenerateCylinder(VertexData, Dimention, Indices);

			m_CylinderBill = std::make_shared<MeshRendererComponent>(
				std::make_shared<TransformComponent>(),
				RenderingSurfaceType::RASTERIZER,
				VertexData, Dimention, Indices,
				std::string(
					#include "../../Shader/ProceduralCity/CylinderBill.vert"
				),
				std::string(
					#include "../../Shader/ProceduralCity/CylinderBill.frag"
				)
			);
		}

		// ストリート
		{
			m_Street = std::make_shared<MeshRendererComponent>(
				std::make_shared<TransformComponent>(),
				PrimitiveType::POINT,
				RenderingSurfaceType::RASTERIZER,
				std::string(
					#include "../../Shader/ProceduralCity/Street.vert"
				),
				std::string(
					#include "../../Shader/ProceduralCity/Street.frag"
				),
				std::string(
					#include "../../Shader/ProceduralCity/Street.geom"
				)
			);
		}

		// Mandelbox
		std::string MandelboxShader = {
			#include "../../Shader/ProceduralCity/MandelboxSample.frag"
		};

		m_Mandelbox = std::make_shared<MeshRendererComponent>(
			std::make_shared<TransformComponent>(),
			PrimitiveType::BOARD,
			RenderingSurfaceType::RAYMARCHING,
			shaderlib::ShaderLib::RaymarchingObject_vert,
			MandelboxShader
			);
	}

	void ProceduralCity::Update() 
	{
		// RPの処理
		m_BillRP->Update();

		// X軸歩道のオフセット
		{
			float PlaneSize = 100.0f;
			float time = GraphicsMain::GetInstance()->time * 0.001f;
			m_XSideWarkVec.z = glm::mod(-time * 11.0f, PlaneSize) - PlaneSize * 0.5;
		}

		/*GraphicsMain::GetInstance()->m_MainCamera->m_position = glm::vec3(
			2.5f * glm::cos(GraphicsMain::GetInstance()->time * 0.001f),
			2.5f,
			2.5f*glm::sin(GraphicsMain::GetInstance()->time*0.001f)
		);
		GraphicsMain::GetInstance()->m_MainCamera->m_center = glm::vec3(0.0f, 2.0f, 0.0f);*/

		// デバッグ用カメラ
		float radius = 2.0f;
		GraphicsMain::GetInstance()->m_MainCamera->m_position = glm::vec3(
			radius * glm::cos(-3.14f/2.0),
			//radius * glm::cos(GraphicsMain::GetInstance()->time * 0.001f),
			2.0f,
			//0.5f,
			radius * glm::sin(-3.14f / 2.0)
			//radius * glm::sin(GraphicsMain::GetInstance()->time * 0.001f)
		);
		//GraphicsMain::GetInstance()->m_MainCamera->m_center = glm::vec3(0.0f, 0.5f, 0.0f);
		GraphicsMain::GetInstance()->m_MainCamera->m_center = glm::vec3(0.0f, 1.0f, 0.0f);

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
			// ビル
			if (m_BillRP)
			{
				m_ProceduralBillRenderer->Draw(GL_PATCHES, true, 1024, [this]() {
					m_ProceduralBillRenderer->m_material->SetVec3Uniform("_ZCenterVec", glm::vec3(0.0f,0.0f,1.0f));
					m_ProceduralBillRenderer->m_material->SetVec3Uniform("XSideWarkVec", m_XSideWarkVec);
					m_ProceduralBillRenderer->m_material->SetFloatUniform("StreetRadius", 2.5f);
					m_ProceduralBillRenderer->m_material->SetFloatUniform("ToSideWarkDist", 2.0f);

					m_BillRP->m_CubeTex->SetActive(GL_TEXTURE1, GL_TEXTURE_CUBE_MAP);
					m_ProceduralBillRenderer->m_material->SetTexUniform("_BillRP", 1);
					});
				m_BillRP->m_CubeTex->SetEnactive(GL_TEXTURE1, GL_TEXTURE_CUBE_MAP);
			}
			
			// 円柱ビル
			if (m_CylinderBill && m_BillRP)
			{
				m_CylinderBill->Draw(GL_TRIANGLES, true, 256, [this]() {
					m_CylinderBill->m_material->SetVec3Uniform("_ZCenterVec", glm::vec3(0.0f, 0.0f, 1.0f));
					m_CylinderBill->m_material->SetVec3Uniform("XSideWarkVec", m_XSideWarkVec);
					m_CylinderBill->m_material->SetFloatUniform("StreetRadius", 2.5f);
					m_CylinderBill->m_material->SetFloatUniform("ToSideWarkDist", 2.0f);

					m_BillRP->m_CubeTex->SetActive(GL_TEXTURE1, GL_TEXTURE_CUBE_MAP);
					m_CylinderBill->m_material->SetTexUniform("_BillRP", 1);
					});
				m_BillRP->m_CubeTex->SetEnactive(GL_TEXTURE1, GL_TEXTURE_CUBE_MAP);
			}

			// ストリート
			if (m_Street)
			{
				m_Street->Draw(GL_POINTS, true, 1024, [this]() {
					m_Street->m_material->SetVec3Uniform("_ZCenterVec", glm::vec3(0.0f, 0.0f, 1.0f));
					m_Street->m_material->SetVec3Uniform("XSideWarkVec", m_XSideWarkVec);
					m_Street->m_material->SetFloatUniform("StreetRadius", 2.5f);
					m_Street->m_material->SetFloatUniform("ToSideWarkDist", 2.0f);
				});
			}
		}
	}
}