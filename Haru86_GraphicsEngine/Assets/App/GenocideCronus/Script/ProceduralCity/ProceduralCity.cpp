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
#include "GraphicsEngine/Graphics/PostProcess.h"
#include "GraphicsEngine/Math/mymath_withGLM.h"
namespace myapp {
	ProceduralCity::ProceduralCity():
		m_BillRP(std::make_shared<ReflectionProbe>(glm::vec3(0.0f, 2.5f, 0.0f), 0.001f)),
		m_Mandelbox(nullptr),
		m_CityCloud(nullptr),
		m_ProceduralBillRenderer(nullptr),
		m_Street(nullptr),
		m_StreeLamp(nullptr),
		m_CylinderBill(nullptr),
		m_XSideWarkVec(glm::vec3(0.0f)),
		m_IsDrawMandel(false),
		m_IsDrawCloud(false),
		m_IsDownSideCloud(false),
		m_GaffDoor(nullptr),
		m_RubbleParticle(nullptr),
		NumOfProBill(1024),
		NumOfCyBill(256),
		m_IsUseBloom(false)
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
		m_Mandelbox = std::make_shared<MeshRendererComponent>(
			std::make_shared<TransformComponent>(),
			PrimitiveType::BOARD,
			RenderingSurfaceType::RAYMARCHING,
			shaderlib::ShaderLib::RaymarchingObject_vert,
			std::string(
				#include "../../Shader/ProceduralCity/MandelboxSample.frag"
			)
		);

		// City Cloud
		m_CityCloud = std::make_shared<MeshRendererComponent>(
			std::make_shared<TransformComponent>(),
			PrimitiveType::BOARD,
			RenderingSurfaceType::RAYMARCHING,
			shaderlib::ShaderLib::RaymarchingObject_vert,
			std::string(
				#include "../../Shader/ProceduralCity/City_Cloud.frag"
			)
		);

		// ガフの扉(みたいなやつ)
		m_GaffDoor = std::make_shared<MeshRendererComponent>(
			std::make_shared<TransformComponent>(),
			PrimitiveType::BOARD,
			RenderingSurfaceType::RASTERIZER,
			shaderlib::ShaderLib::Standard_vert,
			std::string(
				#include "../../Shader/ProceduralCity/GaffDoor.frag"
			)
		);
		m_GaffDoor->m_transform->m_rotation = glm::vec3(-3.14f / 2.0f, 0.0f, 0.0f);
		m_GaffDoor->m_transform->m_scale = glm::vec3(500.0f);
		m_GaffDoor->m_transform->m_position = glm::vec3(0.0f, 100.0f, 0.0f);

		// 瓦礫
		m_RubbleParticle = std::make_shared<MeshRendererComponent>(
			std::make_shared<TransformComponent>(),
			PrimitiveType::SPHERE,
			RenderingSurfaceType::RASTERIZER,
			std::string(
				#include "../../Shader/SacredLake/SacredGPUParticle.vert"
			),
			shaderlib::ShaderLib::Standard_frag
			);

	}

	void ProceduralCity::Update() 
	{
		// RPの処理
		m_BillRP->Update();

		// X軸歩道のオフセット
		{
			float PlaneSize = 100.0f;
			float time = GraphicsMain::GetInstance()->m_SecondsTime;
			m_XSideWarkVec.z = glm::mod(-time * 11.0f, PlaneSize) - PlaneSize * 0.5;
		}
	}

	void ProceduralCity::Draw(bool IsRaymarching, int LinearInstanceRate) {
		//
		bool UseFade = false;

		//
		if (GraphicsMain::GetInstance()->GetAppSceneIndex() == 1)
		{
			UseFade = true;
			NumOfProBill = static_cast<int>(glm::pow(2.0f, 10-LinearInstanceRate));
			NumOfCyBill = static_cast<int>(glm::pow(2.0f, 8 - (glm::max(0, LinearInstanceRate - 2)) ));
		}
		else
		{
			NumOfProBill = 1024;
			NumOfCyBill = 256;
		}
		//
		if (IsRaymarching)
		{
			if (m_IsDrawCloud)
			{
				m_CityCloud->Draw(GL_TRIANGLES, false, 0, [this]() {
					m_CityCloud->m_material->SetIntUniform("_IsDownSideCloud", (m_IsDownSideCloud)? 1 : 0);
				});
			}
			else
			{
				m_Mandelbox->Draw(GL_TRIANGLES, false, 0, [this]() {
					m_Mandelbox->m_material->SetIntUniform("_IsDrawMandel", (m_IsDrawMandel) ? 1 : 0);
				});
			}
		}
		else
		{
			//NumOfProBill = 256;
			//NumOfCyBill = 0;
			// ビル
			if (m_BillRP && NumOfProBill != 1)
			{
				m_ProceduralBillRenderer->Draw(GL_PATCHES, true, NumOfProBill, [=]() {
					m_ProceduralBillRenderer->m_material->SetVec3Uniform("_ZCenterVec", glm::vec3(2.0f * glm::cos(-3.14f / 2.0), 2.0f, 2.0f * glm::sin(-3.14f / 2.0)));
					m_ProceduralBillRenderer->m_material->SetVec3Uniform("XSideWarkVec", m_XSideWarkVec);
					m_ProceduralBillRenderer->m_material->SetFloatUniform("StreetRadius", 2.5f);
					m_ProceduralBillRenderer->m_material->SetFloatUniform("ToSideWarkDist", 1.5f);
					m_ProceduralBillRenderer->m_material->SetIntUniform("_UseFade", (UseFade)? 1 : 0);
					m_ProceduralBillRenderer->m_material->SetIntUniform("_LinearInstanceRate", LinearInstanceRate);
					m_ProceduralBillRenderer->m_material->SetIntUniform("_UseBloom", (m_IsUseBloom)? 1 : 0 );
					m_ProceduralBillRenderer->m_material->SetIntUniform("_IsEndCity", (GraphicsMain::GetInstance()->GetAppSceneIndex() == 6)? 1 : 0 );
					m_ProceduralBillRenderer->m_material->SetIntUniform("_IsParticleBill", 0);

					m_BillRP->m_CubeTex->SetActive(GL_TEXTURE1, GL_TEXTURE_CUBE_MAP);
					m_ProceduralBillRenderer->m_material->SetTexUniform("_BillRP", 1);
					});
				m_BillRP->m_CubeTex->SetEnactive(GL_TEXTURE1, GL_TEXTURE_CUBE_MAP);

				// シーンインデックス６限定の描画
				if (GraphicsMain::GetInstance()->GetAppSceneIndex() == 6)
				{
					// 飛び散るパーティクルビル
					m_ProceduralBillRenderer->Draw(GL_PATCHES, true, NumOfProBill, [=]() {
						m_ProceduralBillRenderer->m_material->SetVec3Uniform("_ZCenterVec", glm::vec3(2.0f * glm::cos(-3.14f / 2.0), 2.0f, 2.0f * glm::sin(-3.14f / 2.0)));
						m_ProceduralBillRenderer->m_material->SetVec3Uniform("XSideWarkVec", m_XSideWarkVec);
						m_ProceduralBillRenderer->m_material->SetFloatUniform("StreetRadius", 2.5f);
						m_ProceduralBillRenderer->m_material->SetFloatUniform("ToSideWarkDist", 1.5f);
						m_ProceduralBillRenderer->m_material->SetIntUniform("_UseFade", (UseFade) ? 1 : 0);
						m_ProceduralBillRenderer->m_material->SetIntUniform("_LinearInstanceRate", LinearInstanceRate);
						m_ProceduralBillRenderer->m_material->SetIntUniform("_IsEndCity", 1);
						m_ProceduralBillRenderer->m_material->SetIntUniform("_IsParticleBill", 1);
						m_ProceduralBillRenderer->m_material->SetIntUniform("_UseBloom", (m_IsUseBloom) ? 1 : 0);

						m_BillRP->m_CubeTex->SetActive(GL_TEXTURE1, GL_TEXTURE_CUBE_MAP);
						m_ProceduralBillRenderer->m_material->SetTexUniform("_BillRP", 1);
						});
					m_BillRP->m_CubeTex->SetEnactive(GL_TEXTURE1, GL_TEXTURE_CUBE_MAP);

					// ガフの扉
					m_GaffDoor->Draw();

					// 瓦礫
					m_RubbleParticle->Draw(GL_TRIANGLES, true, 1024, [this]() {
						m_RubbleParticle->m_material->SetIntUniform("_IDOffset", 0);
						m_RubbleParticle->m_material->SetIntUniform("_NotUseNormal", 1);
						m_RubbleParticle->m_material->SetIntUniform("_IsRandomScale", 1);
						m_RubbleParticle->m_material->SetFloatUniform("_ParticleScale", 1.0f);
						m_RubbleParticle->m_material->SetFloatUniform("_ParticleMoveSpeed", 1.0f);
						});
				}
			}
			
			// 円柱ビル
			if (m_CylinderBill && m_BillRP && NumOfCyBill != 1)
			{
				m_CylinderBill->Draw(GL_TRIANGLES, true, NumOfCyBill, [=]() {
					m_CylinderBill->m_material->SetVec3Uniform("_ZCenterVec", glm::vec3(2.0f * glm::cos(-3.14f / 2.0), 2.0f, 2.0f * glm::sin(-3.14f / 2.0)));
					m_CylinderBill->m_material->SetVec3Uniform("XSideWarkVec", m_XSideWarkVec);
					m_CylinderBill->m_material->SetFloatUniform("StreetRadius", 2.5f);
					m_CylinderBill->m_material->SetFloatUniform("ToSideWarkDist", 1.5f);
					m_CylinderBill->m_material->SetIntUniform("_UseFade", (UseFade) ? 1 : 0);
					m_CylinderBill->m_material->SetIntUniform("_LinearInstanceRate", LinearInstanceRate);
					m_CylinderBill->m_material->SetIntUniform("_IDOffset", 1024 + 1);
					m_CylinderBill->m_material->SetIntUniform("_UseBloom", (m_IsUseBloom)? 1 : 0);
					m_CylinderBill->m_material->SetIntUniform("_IsEndCity", (GraphicsMain::GetInstance()->GetAppSceneIndex() == 6) ? 1 : 0);

					m_BillRP->m_CubeTex->SetActive(GL_TEXTURE1, GL_TEXTURE_CUBE_MAP);
					m_CylinderBill->m_material->SetTexUniform("_BillRP", 1);
					});
				m_BillRP->m_CubeTex->SetEnactive(GL_TEXTURE1, GL_TEXTURE_CUBE_MAP);
			}

			// ストリート
			if (m_Street)
			{
				m_Street->Draw(GL_POINTS, true, 1024, [&]() {
					m_Street->m_material->SetVec3Uniform("_ZCenterVec", glm::vec3(2.0f * glm::cos(-3.14f / 2.0),2.0f, 2.0f * glm::sin(-3.14f / 2.0)));
					m_Street->m_material->SetVec3Uniform("XSideWarkVec", m_XSideWarkVec);
					m_Street->m_material->SetFloatUniform("StreetRadius", 2.5f);
					m_Street->m_material->SetFloatUniform("LocalStreetRadius", 0.4f);
					m_Street->m_material->SetFloatUniform("ToSideWarkDist", 2.5f);
					m_Street->m_material->SetFloatUniform("_pointNum", 1024.0f);
					m_Street->m_material->SetFloatUniform("_Segment", 32.0f);

					m_Street->m_material->SetIntUniform("_LinearInstanceRate", LinearInstanceRate);
					m_Street->m_material->SetIntUniform("_IsEndCity", (GraphicsMain::GetInstance()->GetAppSceneIndex() == 6)? 1 : 0);
				});
			}
		}
	}

	void ProceduralCity::UpdateTimeline(float LocalTime, bool IsReverseTime)
	{
		m_IsDownSideCloud = false;
		m_IsDrawMandel = false;
		m_IsDrawCloud = false;

		if (GraphicsMain::GetInstance()->GetAppSceneIndex() == 0)
		{
			if (LocalTime > 0.0f && LocalTime<31.0f)
			{
				// カメラワーク
				float CameraTimeModeRate = 10.0f, NumOfCamera = 3.0f;
				float CameraworkTime = glm::mod(LocalTime, CameraTimeModeRate * NumOfCamera);

				if (CameraworkTime >= 0.0f && CameraworkTime < CameraTimeModeRate * 1.0f)
				{
					GraphicsMain::GetInstance()->m_MainCamera->m_position = glm::vec3(2.0f * glm::cos(-3.14f / 2.0), 1.0f, 2.0f * glm::sin(-3.14f / 2.0));
					GraphicsMain::GetInstance()->m_MainCamera->m_center = glm::vec3(0.0f, 1.0f, 0.0f);
				}
				else if (CameraworkTime >= CameraTimeModeRate * 1.0f && CameraworkTime < CameraTimeModeRate * 2.0f)
				{
					GraphicsMain::GetInstance()->m_MainCamera->m_position = glm::vec3(0.0f, 10.0f, 10.0f);
					GraphicsMain::GetInstance()->m_MainCamera->m_center = glm::vec3(0.0f, 2.0f, 0.0f);
				}
				else if (CameraworkTime >= CameraTimeModeRate * 2.0f && CameraworkTime < CameraTimeModeRate * 3.0f)
				{
					GraphicsMain::GetInstance()->m_MainCamera->m_position = glm::vec3(2.5f, 0.5f, 2.5f);
					GraphicsMain::GetInstance()->m_MainCamera->m_center = glm::vec3(0.0f, 2.5f, 0.0f);
				}

				//
				//float VignetteRadius = glm::sin(LocalTime);
				float VignetteRadius = glm::clamp((LocalTime - 0.0f) / 5.0f, 0.0f, 1.0f);
				float VignetteLateRadius = glm::clamp((LocalTime - 30.0f)/1.0f, 0.0f, 1.0f);
				float VignetteBrightness = glm::clamp((LocalTime - 30.0f) / 1.0f, 0.0f, 1.0f);
				
				PostProcess::GetInstance()->m_LatePostProcesCallBack = [=]() {
					PostProcess::GetInstance()->m_LateMeshRenderer->m_material->SetIntUniform("_UseVignette", 1);
					PostProcess::GetInstance()->m_LateMeshRenderer->m_material->SetFloatUniform("_VignetteRadius", VignetteRadius);
					PostProcess::GetInstance()->m_LateMeshRenderer->m_material->SetFloatUniform("_VignetteLateRadius", VignetteLateRadius);
					PostProcess::GetInstance()->m_LateMeshRenderer->m_material->SetFloatUniform("_VignetteBrightness", VignetteBrightness);
				};

				m_IsDrawMandel = false;
			}
			else if (LocalTime >= 31.0f && LocalTime < 70.0f)
			{
				// カメラワーク
				float CameraTimeModeRate = 5.0f, NumOfCamera = 5.0f;
				float CameraworkTime = glm::mod(LocalTime, CameraTimeModeRate * NumOfCamera);

				if (CameraworkTime >= 0.0f && CameraworkTime < CameraTimeModeRate * 1.0f)
				{
					GraphicsMain::GetInstance()->m_MainCamera->m_position = glm::vec3(20.0f * glm::cos(LocalTime), 10.0f, 20.0f * glm::sin(LocalTime));
					GraphicsMain::GetInstance()->m_MainCamera->m_center = glm::vec3(0.0f, 2.0f, 0.0f);
				}
				else if (CameraworkTime >= CameraTimeModeRate * 1.0f && CameraworkTime < CameraTimeModeRate * 2.0f)
				{
					GraphicsMain::GetInstance()->m_MainCamera->m_position = glm::vec3(2.0f * glm::cos(-3.14f / 2.0), 1.0f, 2.0f * glm::sin(-3.14f / 2.0));
					GraphicsMain::GetInstance()->m_MainCamera->m_center = glm::vec3(0.0f, 1.0f, 0.0f);
				}
				else if (CameraworkTime >= CameraTimeModeRate * 2.0f && CameraworkTime < CameraTimeModeRate * 3.0f)
				{
					GraphicsMain::GetInstance()->m_MainCamera->m_position = glm::vec3(2.5f, 2.5f, 0.0f);
					GraphicsMain::GetInstance()->m_MainCamera->m_center = glm::vec3(0.0f, 2.5f, 0.0f);
				}
				else if (CameraworkTime >= CameraTimeModeRate * 3.0f && CameraworkTime < CameraTimeModeRate * 4.0f)
				{
					GraphicsMain::GetInstance()->m_MainCamera->m_position = glm::vec3(0.1f, 15.0f, 0.0f);
					GraphicsMain::GetInstance()->m_MainCamera->m_center = glm::vec3(0.0f, 2.5f, 0.0f);
				}
				else if (CameraworkTime >= CameraTimeModeRate * 4.0f && CameraworkTime < CameraTimeModeRate * 5.0f)
				{
					m_IsDownSideCloud = true;
					GraphicsMain::GetInstance()->m_MainCamera->m_position = glm::vec3(2.5f, 0.5f, 2.5f);
					GraphicsMain::GetInstance()->m_MainCamera->m_center = glm::vec3(0.0f, 2.5f, 0.0f);
				}

				PostProcess::GetInstance()->m_LatePostProcesCallBack = []() {
					PostProcess::GetInstance()->m_LateMeshRenderer->m_material->SetIntUniform("_UseVignette", 0);
				};
				m_IsDrawCloud = true;
			}
		}
		else if (GraphicsMain::GetInstance()->GetAppSceneIndex() == 1)
		{
			if (!IsReverseTime && LocalTime > 84.5f)
			{
				PostProcess::GetInstance()->m_LatePostProcesCallBack = [=]() {
					PostProcess::GetInstance()->m_LateMeshRenderer->m_material->SetIntUniform("_UseVignette", 0);
					PostProcess::GetInstance()->m_LateMeshRenderer->m_material->SetIntUniform("_UseWhiteFade", 1);
					PostProcess::GetInstance()->m_LateMeshRenderer->m_material->SetFloatUniform("_WhiteFadeVal", glm::clamp((LocalTime - 84.5f) * 2.0f, 0.0f, 1.0f));
				};
			}
			else if (IsReverseTime && LocalTime > 84.5f && LocalTime <= 85.0f)
			{
				PostProcess::GetInstance()->m_LatePostProcesCallBack = [=]() {
					PostProcess::GetInstance()->m_LateMeshRenderer->m_material->SetIntUniform("_UseVignette", 0);
					PostProcess::GetInstance()->m_LateMeshRenderer->m_material->SetIntUniform("_UseWhiteFade", 1);
					PostProcess::GetInstance()->m_LateMeshRenderer->m_material->SetFloatUniform("_WhiteFadeVal", 1.0f-glm::clamp((85.0f-LocalTime) * 2.0f, 0.0f, 1.0f));
				};
			}
			else if (IsReverseTime && LocalTime > 70.0f && LocalTime <= 72.0f)
			{
				PostProcess::GetInstance()->m_LatePostProcesCallBack = [=]() {
					PostProcess::GetInstance()->m_LateMeshRenderer->m_material->SetIntUniform("_UseVignette", 0);
					PostProcess::GetInstance()->m_LateMeshRenderer->m_material->SetIntUniform("_UseWhiteFade", 1);
					PostProcess::GetInstance()->m_LateMeshRenderer->m_material->SetFloatUniform("_WhiteFadeVal", glm::clamp((72.0f-LocalTime) / 2.0f, 0.0f, 1.0f));
				};
			}
			else
			{
				PostProcess::GetInstance()->m_LatePostProcesCallBack = [=]() {
					PostProcess::GetInstance()->m_LateMeshRenderer->m_material->SetIntUniform("_UseVignette", 0);
					PostProcess::GetInstance()->m_LateMeshRenderer->m_material->SetIntUniform("_UseWhiteFade", 0);
				};
			}
			
			m_IsDrawCloud = true;

			GraphicsMain::GetInstance()->m_MainCamera->m_position = glm::vec3(2.0f * glm::cos(-3.14f / 2.0), 1.0f, 2.0f * glm::sin(-3.14f / 2.0));
			GraphicsMain::GetInstance()->m_MainCamera->m_center = glm::vec3(0.0f, 1.0f, 0.0f);
		}
		else if (GraphicsMain::GetInstance()->GetAppSceneIndex() == 6)
		{
			m_IsDrawCloud = false;
			m_IsDrawMandel = true;

			if (LocalTime < 246.0f) {
				// カメラワーク
				float CameraTimeModeRate = 10.0f, NumOfCamera = 5.0f;
				float CameraworkTime = glm::mod(LocalTime, CameraTimeModeRate * NumOfCamera);
				m_Street->m_transform->m_scale = glm::vec3(1.0f);

				if (CameraworkTime >= 0.0f && CameraworkTime < CameraTimeModeRate * 1.0f)
				{
					GraphicsMain::GetInstance()->m_MainCamera->m_position = glm::vec3(0.0f, 0.5f, 2.5f * 10.0f); // 固定
					GraphicsMain::GetInstance()->m_MainCamera->m_center = glm::vec3(0.0f, 5.0f, 0.0f);
				}
				else if (CameraworkTime >= CameraTimeModeRate * 1.0f && CameraworkTime < CameraTimeModeRate * 2.0f)
				{
					GraphicsMain::GetInstance()->m_MainCamera->m_center = glm::vec3(0.0f, 10.0f, 0.0f); // SacredLake // 高い回転
					GraphicsMain::GetInstance()->m_MainCamera->m_position = glm::vec3( // SacredLake
						glm::cos(LocalTime * 0.1f) * 15.0f,
						50.0f,
						glm::sin(LocalTime * 0.1f) * 15.0f
					);
				}
				else if (CameraworkTime >= CameraTimeModeRate * 2.0f && CameraworkTime < CameraTimeModeRate * 3.0f)
				{
					GraphicsMain::GetInstance()->m_MainCamera->m_position = glm::vec3(20.0f * glm::cos(LocalTime * 0.1f), 10.0f, 20.0f * glm::sin(LocalTime * 0.1f));
					GraphicsMain::GetInstance()->m_MainCamera->m_center = glm::vec3(0.0f, 2.0f, 0.0f); // よさげな回転
				}
				else if (CameraworkTime >= CameraTimeModeRate * 3.0f && CameraworkTime < CameraTimeModeRate * 4.0f) 
				{
					GraphicsMain::GetInstance()->m_MainCamera->m_position = glm::vec3(2.5f, 0.5f, 0.0f); // 近くていい感じの固定
					GraphicsMain::GetInstance()->m_MainCamera->m_center = glm::vec3(0.0f, 5.0f, 0.0f);
				}
				else if (CameraworkTime >= CameraTimeModeRate * 4.0f && CameraworkTime < CameraTimeModeRate * 5.0f)
				{
					m_Street->m_transform->m_scale = glm::vec3(100.0f, 1.0f, 100.0f);
					GraphicsMain::GetInstance()->m_MainCamera->m_position = glm::vec3(50.5f, 10.5f, 50.5f); // 広い視野で固定
					GraphicsMain::GetInstance()->m_MainCamera->m_center = glm::vec3(0.0f, 2.5f, 0.0f);
				}
			}
			else if(LocalTime >= 246.0f)
			{
				// ブルームをオン
				PostProcess::GetInstance()->m_UseBloom = true;
				m_IsUseBloom = true;

				// ブルームパラメーター
				PostProcess::GetInstance()->m_BloomThreshold = 2.0f * glm::clamp((LocalTime - 246.0f) / 14.0f, 0.0f, 1.0f);
				PostProcess::GetInstance()->m_BloomIntensity = 2.0f * glm::clamp((LocalTime - 246.0f) / 4.0f, 0.0f, 1.0f)
					+ 400.0f * glm::clamp((LocalTime - 254.0f) / 8.0f, 0.0f, 1.0f);

				// カメラワーク
				float CameraTimeModeRate = 5.0f, NumOfCamera = 3.0f;
				float CameraworkTime = glm::mod(LocalTime, CameraTimeModeRate * NumOfCamera);
				m_Street->m_transform->m_scale = glm::vec3(1.0f);

				if (CameraworkTime >= 0.0f && CameraworkTime < CameraTimeModeRate * 1.0f)
				{
					GraphicsMain::GetInstance()->m_MainCamera->m_position = glm::vec3(0.0f, 0.5f, 2.5f * 10.0f); // 固定
					GraphicsMain::GetInstance()->m_MainCamera->m_center = glm::vec3(0.0f, 5.0f, 0.0f);
				}
				else if (CameraworkTime >= CameraTimeModeRate * 1.0f && CameraworkTime < CameraTimeModeRate * 2.0f)
				{
					m_Street->m_transform->m_scale = glm::vec3(100.0f, 1.0f, 100.0f);
					GraphicsMain::GetInstance()->m_MainCamera->m_position = glm::vec3(50.5f, 10.5f, 50.5f); // 広い視野で固定
					GraphicsMain::GetInstance()->m_MainCamera->m_center = glm::vec3(0.0f, 2.5f, 0.0f);
				}
				else if (CameraworkTime >= CameraTimeModeRate * 2.0f && CameraworkTime < CameraTimeModeRate * 3.0f)
				{
					GraphicsMain::GetInstance()->m_MainCamera->m_position = glm::vec3(20.0f * glm::cos(LocalTime * 0.1f), 10.0f, 20.0f * glm::sin(LocalTime * 0.1f));
					GraphicsMain::GetInstance()->m_MainCamera->m_center = glm::vec3(0.0f, 2.0f, 0.0f); // よさげな回転
				}
			}
			
			PostProcess::GetInstance()->m_LatePostProcesCallBack = [=]() {
				PostProcess::GetInstance()->m_LateMeshRenderer->m_material->SetIntUniform("_UseVignette", 0);
				PostProcess::GetInstance()->m_LateMeshRenderer->m_material->SetIntUniform("_UseThirdImpact", 1);
				PostProcess::GetInstance()->m_LateMeshRenderer->m_material->SetIntUniform("_UseWhiteNoise", (LocalTime <= 254.0f)? 1 : 0);
				PostProcess::GetInstance()->m_LateMeshRenderer->m_material->SetIntUniform("_UseFilmFilter", 1);
				PostProcess::GetInstance()->m_LateMeshRenderer->m_material->SetIntUniform("_UseWave", 1);
				PostProcess::GetInstance()->m_LateMeshRenderer->m_material->SetFloatUniform("_WaveSpeed", (LocalTime <= 254.0f) ? 1.0f : 1000.0f);
				PostProcess::GetInstance()->m_LateMeshRenderer->m_material->SetFloatUniform("_WaveWidth", (LocalTime <= 254.0f) ? 1.0f : 10.0f);
				PostProcess::GetInstance()->m_LateMeshRenderer->m_material->SetFloatUniform("_FilmWidthReduction", 0.2f*glm::clamp((LocalTime - 254.0f)/4.0f,0.0f,1.0f));
				PostProcess::GetInstance()->m_LateMeshRenderer->m_material->SetIntUniform("_UseRewinding", 0);
				
				if (LocalTime <= 179.0f)
				{
					PostProcess::GetInstance()->m_LateMeshRenderer->m_material->SetIntUniform("_UseWhiteFade", 1);
					PostProcess::GetInstance()->m_LateMeshRenderer->m_material->SetFloatUniform("_WhiteFadeVal", 1.0f - glm::clamp((LocalTime - 177.0f)/2.0f, 0.0f, 1.0f));
					//PostProcess::GetInstance()->m_LateMeshRenderer->m_material->SetFloatUniform("_WhiteFadeVal", 1.0f);
				}
				if (LocalTime > 254.0f)
				{
					PostProcess::GetInstance()->m_LateMeshRenderer->m_material->SetIntUniform("_UseWhiteFade", 1);
					PostProcess::GetInstance()->m_LateMeshRenderer->m_material->SetFloatUniform("_WhiteFadeVal", glm::clamp((LocalTime - 254.0f)/4.0f, 0.0f, 1.0f));
				}
				else
				{
					PostProcess::GetInstance()->m_LateMeshRenderer->m_material->SetIntUniform("_UseWhiteFade", 0);
				}
			};
		}
	}
}