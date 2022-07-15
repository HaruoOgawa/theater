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

namespace myapp {
	SacredLake::SacredLake():
		m_Mandelbox(nullptr),
		m_GPUMaterial(nullptr),
		m_GPUTRS(nullptr),
		m_GPUParticleMesh(nullptr)
	{
		Start();
	}

	void SacredLake::Start() {
		// Object
		std::string SacredLake_WaterReflection_vert = {
			#include "../Shader/SacredLake_WaterReflection.vert"
		};

		std::string SacredLake_WaterReflection_frag = {
			#include "../Shader/SacredLake_WaterReflection.frag"
		};

		std::string SacredLake_WaterReflection_tesc = {
			#include "../Shader/SacredLake_WaterReflection.tesc"
		};

		std::string SacredLake_WaterReflection_tese = {
			#include "../Shader/SacredLake_WaterReflection.tese"
		};

		std::string SacredLake_WaterReflection_geom = {
			#include "../Shader/SacredLake_WaterReflection.geom"
		};
		
		//m_ReflectPlaneMaterial = std::make_shared<Material>(RenderingSurfaceType::RASTERIZER, SacredLake_WaterReflection_vert, SacredLake_WaterReflection_frag,
		//	SacredLake_WaterReflection_geom, SacredLake_WaterReflection_tesc, SacredLake_WaterReflection_tese);
		m_ReflectPlaneMaterial = std::make_shared<Material>(RenderingSurfaceType::RASTERIZER, shaderlib::ShaderLib::Standard_vert,shaderlib::ShaderLib::Standard_frag);
		m_ReflectPlaneTRS = std::make_shared<TransformComponent>();
		m_ReflectPlaneTRS->m_rotation = glm::vec3(-3.14f / 2.0f, 0.0f, 0.0f);
		m_ReflectPlaneTRS->m_scale = glm::vec3(500.0f);
		//m_ReflectPlaneTRS->m_scale = glm::vec3(15.0f);
		m_ReflectPlaneTRS->m_position = glm::vec3(0.0f, -10.0f, 0.0f);
		m_ReflectPlaneMesh = std::make_shared<Mesh>(PrimitiveType::BOARD);

		// VolumetricCloud
		std::string VolumetricCloud_frag = {
			#include "../Shader/VolumetricCloud.frag"
		};
		m_VolumetricCloud= std::make_shared<GameObject>(
			PrimitiveType::BOARD,
			RenderType::DefaultBuffer,
			RenderQueue::Geometry,
			RenderingSurfaceType::RAYMARCHING,
			shaderlib::ShaderLib::RaymarchingObject_vert,
			VolumetricCloud_frag
			);
		m_VolumetricCloud->meshComp->useZTest = false;

		// Mandelbox
		std::string MandelboxShader = {
			#include "../Shader/SacredLake_Mandelbox.frag"
		};

		m_Mandelbox = std::make_shared<GameObject>(
			PrimitiveType::BOARD,
			RenderType::DefaultBuffer,
			RenderQueue::Geometry,
			RenderingSurfaceType::RAYMARCHING,
			shaderlib::ShaderLib::RaymarchingObject_vert,
			MandelboxShader
			);
		m_Mandelbox->meshComp->useAlphaTest = true;
		m_Mandelbox->meshComp->useZTest = false;

		// GPU particle
		std::string GPUVert = {
			#include "../Shader/SacredGPUParticle.vert"
		};
		std::string GPUFrag = {
			#include "../Shader/SacredGPUParticle.frag"
		};
		std::string GPUGeom = {
			#include "../Shader/SacredGPUParticle.geom"
		};
		m_GPUMaterial = std::make_shared<Material>(RenderingSurfaceType::RASTERIZER, GPUVert, GPUFrag, GPUGeom);

		m_GPUParticleMesh = std::make_shared<Mesh>(PrimitiveType::POINT);
		m_GPUTRS = std::make_shared<TransformComponent>();
	}

	void SacredLake::Update() 
	{
	}

	void SacredLake::Draw() {
		// GPU particle
		/*m_GPUMaterial->SetActive();
		m_GPUTRS->CalMatrix();
		m_GPUMaterial->SetMatrixUniform("MVPMatrix", m_GPUTRS->m_pMatrix * m_GPUTRS->m_vMatrix * m_GPUTRS->m_mMatrix);
		m_GPUMaterial->SetMatrixUniform("MMatrix", m_GPUTRS->m_mMatrix);
		m_GPUMaterial->SetMatrixUniform("VMatrix", m_GPUTRS->m_vMatrix);
		m_GPUMaterial->SetMatrixUniform("PMatrix", m_GPUTRS->m_pMatrix);
		m_GPUMaterial->SetVec2Uniform("_resolution", GraphicsRenderer::GetInstance()->GetScreenSize());
		m_GPUMaterial->SetFloatUniform("_frameResolusion", GraphicsRenderer::GetInstance()->frameResolusion);
		m_GPUMaterial->SetFloatUniform("_time", GraphicsMain::GetInstance()->time*0.001f);
		m_GPUMaterial->SetIntUniform("_NotUseNormal", 1);
		m_GPUParticleMesh->DrawInstancedWithMesh(1024, GL_POINTS);*/

		if (GraphicsMain::GetInstance()->m_UsingCamera== GraphicsMain::GetInstance()->m_MainCamera) {
			// Plane
			m_ReflectPlaneMaterial->SetActive();
			m_ReflectPlaneTRS->CalMatrix();
			m_ReflectPlaneMaterial->SetMatrixUniform("MVPMatrix", m_ReflectPlaneTRS->m_pMatrix * m_ReflectPlaneTRS->m_vMatrix * m_ReflectPlaneTRS->m_mMatrix);
			m_ReflectPlaneMaterial->SetMatrixUniform("MMatrix", m_ReflectPlaneTRS->m_mMatrix);
			m_ReflectPlaneMaterial->SetMatrixUniform("VMatrix", m_ReflectPlaneTRS->m_vMatrix);
			m_ReflectPlaneMaterial->SetMatrixUniform("PMatrix", m_ReflectPlaneTRS->m_pMatrix);
			m_ReflectPlaneMaterial->SetVec2Uniform("_resolution", GraphicsRenderer::GetInstance()->GetScreenSize());
			m_ReflectPlaneMaterial->SetFloatUniform("_frameResolusion", GraphicsRenderer::GetInstance()->frameResolusion);
			m_ReflectPlaneMaterial->SetVec3Uniform("_CameraPos", GraphicsMain::GetInstance()->m_MainCamera->m_position);
			m_ReflectPlaneMaterial->SetFloatUniform("_time", GraphicsMain::GetInstance()->time*0.001f);

			m_ReflectPlaneMaterial->SetIntUniform("_UseColor", 1);
			m_ReflectPlaneMaterial->SetVec4Uniform("_Color", glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
			//m_ReflectPlaneMesh->Draw(GL_PATCHES);
			m_ReflectPlaneMesh->Draw();
		}
	}
}