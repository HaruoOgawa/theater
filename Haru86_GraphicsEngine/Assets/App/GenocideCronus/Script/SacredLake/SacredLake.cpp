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
		m_ReflectPlane(nullptr)
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
			PrimitiveType::POINT,
			RenderingSurfaceType::RASTERIZER,
			std::string(
				#include "../../Shader/SacredLake/SacredGPUParticle.vert"
			),std::string(
				#include "../../Shader/SacredLake/SacredGPUParticle.frag"
			),std::string(
				#include "../../Shader/SacredLake/SacredGPUParticle.geom"
			)
		);
	}

	void SacredLake::Update() 
	{
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
			m_GPUParticle->Draw(GL_POINTS, true, 1024);

			//
			if (GraphicsMain::GetInstance()->m_UsingCamera == GraphicsMain::GetInstance()->m_MainCamera) {
				m_ReflectPlane->Draw(GL_TRIANGLES, false, 0, [this]() {
					m_ReflectPlane->m_material->SetIntUniform("_UseColor", 1);
					m_ReflectPlane->m_material->SetVec4Uniform("_Color", glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
				});
			}
		}
	}
}