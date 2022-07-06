#include "SacredLake.h"
#include "GraphicsEngine/Graphics/GraphicsRenderer.h"
#include "GraphicsEngine/Graphics/Mesh.h"
#include "GraphicsEngine/Graphics/Material.h"
#include "GraphicsEngine/Graphics/ShaderLib.h"
#include "GraphicsEngine/Component/TransformComponent.h"
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
		std::string SacredLake_WaterReflection = {
			#include "../Shader/SacredLake_WaterReflection.frag"
		};
		
		m_ReflectPlaneMaterial = std::make_shared<Material>(RenderingSurfaceType::RASTERIZER, shaderlib::ShaderLib::Standard_vert, SacredLake_WaterReflection);
		m_ReflectPlaneTRS = std::make_shared<TransformComponent>();
		m_ReflectPlaneTRS->m_rotation = glm::vec3(3.14f / 2.0f, 0.0f, 0.0f);
		m_ReflectPlaneTRS->m_scale = glm::vec3(500.0f);
		m_ReflectPlaneTRS->m_position = glm::vec3(0.0f, -10.0f, -100.0f);
		m_ReflectPlaneMesh = std::make_shared<Mesh>(PrimitiveType::BOARD);

		//
		m_ReflectSphereTRS = std::make_shared<TransformComponent>();
		m_ReflectSphereTRS->m_scale = glm::vec3(7.0f);
		m_ReflectSphereTRS->m_position = glm::vec3(0.0f, 10.0f, 0.0f);
		m_ReflectSphereMesh = std::make_shared<Mesh>(PrimitiveType::SPHERE);
	
		// raymarching
		std::string MandelboxShader = {
			#include "../Shader/SacredLake_Mandelbox.frag"
		};

		/*m_Mandelbox = std::make_shared<GameObject>(
			PrimitiveType::BOARD,
			RenderType::DefaultBuffer,
			RenderQueue::Geometry,
			RenderingSurfaceType::RAYMARCHING,
			shaderlib::ShaderLib::RaymarchingObject_vert,
			MandelboxShader
			);*/

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

		// リフレクションプローブ
		m_RP = std::make_shared<ReflectionProbe>();
		GraphicsMain::GetInstance()->m_ReflectionProbeList.push_back(m_RP);

		// モノディル
		glm::vec3 viewDir = m_ReflectPlaneTRS->m_position - GraphicsMain::GetInstance()->m_MainCamera->m_position;
		glm::vec3 refDir = glm::reflect(-glm::normalize(viewDir), glm::vec3(0.0f, 1.0f, 0.0f));
		std::shared_ptr<TransformComponent> MonoDirTRS = std::make_shared<TransformComponent>();
		MonoDirTRS->m_position = m_ReflectPlaneTRS->m_position+glm::vec3(0.0f,10.0f,0.0f);
		MonoDirTRS->m_center = m_ReflectPlaneTRS->m_position + refDir + glm::vec3(0.0f, 10.0f, 0.0f);

		m_MonoDirRP = std::make_shared<ReflectionProbe>(MonoDirTRS);
		GraphicsMain::GetInstance()->m_ReflectionProbeList.push_back(m_MonoDirRP);
	}

	void SacredLake::Update() {
		// GPU particle
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

			m_RP->m_CubeTex->SetActive(GL_TEXTURE0,GL_TEXTURE_CUBE_MAP);
			m_ReflectPlaneMaterial->SetTexUniform("_WaterRP", 0);

			m_MonoDirRP->m_CubeTex->SetActive(GL_TEXTURE1, GL_TEXTURE_2D);
			m_ReflectPlaneMaterial->SetTexUniform("_MonoDirRP", 1);

			m_ReflectPlaneMesh->Draw();
			m_RP->m_CubeTex->SetEnactive(GL_TEXTURE0, GL_TEXTURE_CUBE_MAP);
			m_MonoDirRP->m_CubeTex->SetEnactive(GL_TEXTURE1, GL_TEXTURE_2D);
			
			// Sphere
			/*m_ReflectPlaneMaterial->SetActive();
			m_ReflectSphereTRS->CalMatrix();
			m_ReflectPlaneMaterial->SetMatrixUniform("MVPMatrix", m_ReflectSphereTRS->m_pMatrix * m_ReflectSphereTRS->m_vMatrix * m_ReflectSphereTRS->m_mMatrix);
			m_ReflectPlaneMaterial->SetMatrixUniform("MMatrix", m_ReflectSphereTRS->m_mMatrix);
			m_ReflectPlaneMaterial->SetMatrixUniform("VMatrix", m_ReflectSphereTRS->m_vMatrix);
			m_ReflectPlaneMaterial->SetMatrixUniform("PMatrix", m_ReflectSphereTRS->m_pMatrix);
			m_ReflectPlaneMaterial->SetVec2Uniform("_resolution", GraphicsRenderer::GetInstance()->GetScreenSize());
			m_ReflectPlaneMaterial->SetFloatUniform("_frameResolusion", GraphicsRenderer::GetInstance()->frameResolusion);
			m_ReflectPlaneMaterial->SetVec3Uniform("_CameraPos", GraphicsMain::GetInstance()->m_MainCamera->m_position);

			m_RP->m_CubeTex->SetActive(GL_TEXTURE0, GL_TEXTURE_CUBE_MAP);
			m_ReflectPlaneMaterial->SetTexUniform("_WaterRP", 0);

			m_MonoDirRP->m_CubeTex->SetActive(GL_TEXTURE1, GL_TEXTURE_2D);
			m_ReflectPlaneMaterial->SetTexUniform("_MonoDirRP", 1);

			m_ReflectSphereMesh->Draw();
			m_RP->m_CubeTex->SetEnactive(GL_TEXTURE0, GL_TEXTURE_CUBE_MAP);	
			m_MonoDirRP->m_CubeTex->SetEnactive(GL_TEXTURE1, GL_TEXTURE_2D);*/
		}
	}
}