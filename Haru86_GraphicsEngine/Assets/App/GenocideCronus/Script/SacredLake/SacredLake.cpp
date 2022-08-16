#include "SacredLake.h"
#include "GraphicsEngine/Graphics/GraphicsRenderer.h"
#include "GraphicsEngine/Graphics/Mesh.h"
#include "GraphicsEngine/Graphics/Material.h"
#include "GraphicsEngine/Graphics/ShaderLib.h"
#include "GraphicsEngine/Component/TransformComponent.h"
#include "GraphicsEngine/Component/MeshRendererComponent.h"
#include "GraphicsEngine/Message/Console.h"
#include "GraphicsEngine/Object/GameObject.h"
#include "GraphicsEngine/Graphics/ReflectionProbe.h"
#include "GraphicsEngine/Graphics/Texture.h"
#include "GraphicsEngine/Graphics/PostProcess.h"

namespace myapp {
	SacredLake::SacredLake():
		m_Mandelbox(nullptr),
		m_ReflectPlane(nullptr),
		m_ReflectionProbe(std::make_shared<ReflectionProbe>(glm::vec3(0.0f, 50.0f, 0.0f), 0.001f))
	{
		Start();
	}

	void SacredLake::Start() {
		// Reflection Plane
		m_ReflectPlane = std::make_shared<MeshRendererComponent>(
			std::make_shared<TransformComponent>(),
			PrimitiveType::BOARD,
			RenderingSurfaceType::RASTERIZER,
			shaderlib::ShaderLib::Standard_vert,
			shaderlib::ShaderLib::Standard_frag
		);
		m_ReflectPlane->m_transform->m_rotation = glm::vec3(-3.14f / 2.0f, 0.0f, 0.0f);
		m_ReflectPlane->m_transform->m_scale = glm::vec3(500.0f);
		m_ReflectPlane->m_transform->m_position = glm::vec3(0.0f, -10.0f, 0.0f);

		// VolumetricCloud
		std::string VolumetricCloud_frag = {
			#include "../../Shader/SacredLake/VolumetricCloud.frag"
		};
		m_VolumetricCloud= std::make_shared<MeshRendererComponent>(
			std::make_shared<TransformComponent>(),
			PrimitiveType::BOARD,
			RenderingSurfaceType::RAYMARCHING,
			shaderlib::ShaderLib::RaymarchingObject_vert,
			VolumetricCloud_frag
			);
		m_VolumetricCloud->useZTest = false;
		m_VolumetricCloud->useAlphaTest = false;

		// Mandelbox
		m_Mandelbox = std::make_shared<MeshRendererComponent>(
			std::make_shared<TransformComponent>(),
			PrimitiveType::BOARD,
			RenderingSurfaceType::RAYMARCHING,
			shaderlib::ShaderLib::RaymarchingObject_vert,
			std::string(
				#include "../../Shader/SacredLake/SacredLake_Mandelbox.frag"
			)
		);
		m_Mandelbox->useAlphaTest = true;
		m_Mandelbox->useZTest = false;

		// GPU particle
		m_GPUParticle = std::make_shared<MeshRendererComponent>(
			std::make_shared<TransformComponent>(),
			PrimitiveType::CUBE,
			RenderingSurfaceType::RASTERIZER,
			std::string(
				#include "../../Shader/SacredLake/SacredGPUParticle.vert"
			),
			shaderlib::ShaderLib::Standard_frag
		);
		
		// m_SphereGPUParticle
		m_SphereGPUParticle = std::make_shared<MeshRendererComponent>(
			std::make_shared<TransformComponent>(),
			PrimitiveType::SPHERE,
			RenderingSurfaceType::RASTERIZER,
			shaderlib::ShaderLib::Standard_vert,
			/*std::string(
				#include "../../Shader/SacredLake/SacredGPUParticle.vert"
			),*/
			shaderlib::ShaderLib::Standard_frag
		);

		m_SphereGPUParticle->m_transform->m_scale = glm::vec3(1.0f);
		m_SphereGPUParticle->m_transform->m_position = glm::vec3(25.0f,0.0f,0.0f);
	}

	void SacredLake::Update() 
	{
		m_ReflectionProbe->Update();
	}

	void SacredLake::Draw(bool IsRaymarching) {
		PostProcess::GetInstance()->m_UseSSR = true;

		if (IsRaymarching)
		{
			//
			m_VolumetricCloud->Draw();
			m_Mandelbox->Draw();
		}
		else
		{
			// GPU particle
			if (m_ReflectionProbe->GetRPProgress()==EReflectionProbeProgress::End && m_GPUParticle) {
				m_GPUParticle->Draw(GL_TRIANGLES, true, 1024, [this]() {
					m_GPUParticle->m_material->SetIntUniform("_IDOffset", 0);
					m_GPUParticle->m_material->SetIntUniform("_NotUseNormal", 1);
					//m_GPUParticle->m_material->SetIntUniform("_UseLighting", 0);
					m_GPUParticle->m_material->SetFloatUniform("_ParticleScale", 2.0f);

					/*m_GPUParticle->m_material->SetIntUniform("_UseMainCube", 1);
					m_ReflectionProbe->m_CubeTex->SetActive(GL_TEXTURE1, GL_TEXTURE_CUBE_MAP);
					m_GPUParticle->m_material->SetTexUniform("_MainCube", 1);*/
				});
				//m_ReflectionProbe->m_CubeTex->SetEnactive(GL_TEXTURE1, GL_TEXTURE_CUBE_MAP);
			}
			
			// m_SphereGPUParticle
			//if (m_ReflectionProbe->GetRPProgress() == EReflectionProbeProgress::End && m_SphereGPUParticle) {
			//	m_SphereGPUParticle->Draw(GL_TRIANGLES, false, 256, [this]() {
			//	//m_SphereGPUParticle->Draw(GL_TRIANGLES, true, 256, [this]() {
			//		m_SphereGPUParticle->m_material->SetIntUniform("_IDOffset", 1024+100);
			//		m_SphereGPUParticle->m_material->SetIntUniform("_NotUseNormal", 1);
			//		m_SphereGPUParticle->m_material->SetFloatUniform("_ParticleScale", 0.75f);

			//		m_SphereGPUParticle->m_material->SetIntUniform("_UseMainCube", 1);
			//		m_ReflectionProbe->m_CubeTex->SetActive(GL_TEXTURE1, GL_TEXTURE_CUBE_MAP);
			//		m_SphereGPUParticle->m_material->SetTexUniform("_MainCube", 1);
			//		});
			//	m_ReflectionProbe->m_CubeTex->SetEnactive(GL_TEXTURE1, GL_TEXTURE_CUBE_MAP);
			//}

			//
			//if (GraphicsMain::GetInstance()->m_UsingCamera == GraphicsMain::GetInstance()->m_MainCamera)
			{
				m_ReflectPlane->Draw(GL_TRIANGLES, false, 0, [this]() {
					m_ReflectPlane->m_material->SetIntUniform("_UseColor", 1);
					m_ReflectPlane->m_material->SetVec4Uniform("_Color", glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
				});
			}
		}
	}
}