#include "SacredLake.h"
#include "GraphicsEngine/Graphics/GraphicsRenderer.h"
#include "GraphicsEngine/Graphics/Mesh.h"
#include "GraphicsEngine/Graphics/Material.h"
#include "GraphicsEngine/Graphics/ShaderLib.h"
#include "GraphicsEngine/Component/TransformComponent.h"
#include "GraphicsEngine/Message/Console.h"
#include "GraphicsEngine/Object/GameObject.h"
#include "GraphicsEngine/Graphics/RealtimeReflectionProbe.h"
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
		/*m_ReflectPlane = std::make_shared<GameObject>(PrimitiveType::BOARD, RenderType::DefaultBuffer,
			RenderQueue::Geometry,RenderingSurfaceType::RASTERIZER,
			shaderlib::ShaderLib::Standard_vert, SacredLake_WaterReflection);
		m_ReflectPlane->m_transform->m_rotation = glm::vec3(3.14f / 2.0f, 0.0f, 0.0f);
		m_ReflectPlane->m_transform->m_scale = glm::vec3(100.0f);
		m_ReflectPlane->m_transform->m_position = glm::vec3(0.0f, -1.0f, 0.0f);*/
		m_ReflectPlaneMaterial = std::make_shared<Material>(RenderingSurfaceType::RASTERIZER,shaderlib::ShaderLib::Standard_vert,SacredLake_WaterReflection);
		m_ReflectPlaneTRS = std::make_shared<TransformComponent>();
		m_ReflectPlaneTRS->m_rotation = glm::vec3(3.14f / 2.0f, 0.0f, 0.0f);
		m_ReflectPlaneTRS->m_scale = glm::vec3(100.0f);
		m_ReflectPlaneTRS->m_position = glm::vec3(0.0f, -1.0f, 0.0f);
		m_ReflectPlaneMesh = std::make_shared<Mesh>(PrimitiveType::BOARD);

		// raymarching
		/*std::string MandelboxShader = {
			#include "../Shader/SacredLake_Mandelbox.frag"
		};

		m_Mandelbox = std::make_shared<GameObject>(
			PrimitiveType::BOARD,
			RenderType::DefaultBuffer,
			RenderQueue::Geometry,
			RenderingSurfaceType::RAYMARCHING,
			shaderlib::ShaderLib::RaymarchingObject_vert,
			MandelboxShader
			);*/

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

		// リアルタイムリフレクションプローブ
		m_RP = std::make_shared<RealtimeReflectionProbe>();
		GraphicsMain::GetInstance()->m_RealtimeReflectionProbe = m_RP;
	}

	void SacredLake::Update() {
		// GPU particle
	}

	void SacredLake::Draw() {
		// GPU particle
		m_GPUMaterial->SetActive();
		m_GPUTRS->CalMatrix();
		m_GPUMaterial->SetMatrixUniform("MVPMatrix", m_GPUTRS->m_pMatrix * m_GPUTRS->m_vMatrix * m_GPUTRS->m_mMatrix);
		m_GPUMaterial->SetMatrixUniform("MMatrix", m_GPUTRS->m_mMatrix);
		m_GPUMaterial->SetMatrixUniform("VMatrix", m_GPUTRS->m_vMatrix);
		m_GPUMaterial->SetMatrixUniform("PMatrix", m_GPUTRS->m_pMatrix);
		m_GPUMaterial->SetVec2Uniform("_resolution", GraphicsRenderer::GetInstance()->GetScreenSize());
		m_GPUMaterial->SetFloatUniform("_frameResolusion", GraphicsRenderer::GetInstance()->frameResolusion);

		m_GPUParticleMesh->DrawInstancedWithMesh(1024, GL_POINTS);

		//
		m_ReflectPlaneMaterial->SetActive();
		m_ReflectPlaneTRS->CalMatrix();
		m_ReflectPlaneMaterial->SetMatrixUniform("MVPMatrix", m_ReflectPlaneTRS->m_pMatrix * m_ReflectPlaneTRS->m_vMatrix * m_ReflectPlaneTRS->m_mMatrix);
		m_ReflectPlaneMaterial->SetMatrixUniform("MMatrix", m_ReflectPlaneTRS->m_mMatrix);
		m_ReflectPlaneMaterial->SetMatrixUniform("VMatrix", m_ReflectPlaneTRS->m_vMatrix);
		m_ReflectPlaneMaterial->SetMatrixUniform("PMatrix", m_ReflectPlaneTRS->m_pMatrix);
		m_ReflectPlaneMaterial->SetVec2Uniform("_resolution", GraphicsRenderer::GetInstance()->GetScreenSize());
		m_ReflectPlaneMaterial->SetFloatUniform("_frameResolusion", GraphicsRenderer::GetInstance()->frameResolusion);
		
		//m_RP->m_CubeTexList[1]->SetActive(GL_TEXTURE0);
		//m_ReflectPlaneMaterial->SetTexUniform("_WaterRP", 0);
		
		m_ReflectPlaneMesh->Draw();
		//m_RP->m_CubeTexList[1]->SetEnactive(GL_TEXTURE0);
	}
}