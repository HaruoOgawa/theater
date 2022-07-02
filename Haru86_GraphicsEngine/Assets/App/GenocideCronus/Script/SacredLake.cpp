#include "SacredLake.h"
#include "GraphicsEngine/Graphics/GraphicsRenderer.h"
#include "GraphicsEngine/Graphics/Texture.h"
#include "GraphicsEngine/Graphics/Mesh.h"
#include "GraphicsEngine/Graphics/Material.h"
#include "GraphicsEngine/Graphics/ShaderLib.h"
#include "GraphicsEngine/Component/TransformComponent.h"
#include "BillWindowGenerator.h"
#include "GraphicsEngine/Message/Console.h"
#include "GraphicsEngine/Object/GameObject.h"

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
		m_ReflectPlane = std::make_shared<GameObject>(PrimitiveType::BOARD);
		m_ReflectPlane->m_transform->m_rotation = glm::vec3(3.14f / 2.0f, 0.0f, 0.0f);
		m_ReflectPlane->m_transform->m_scale = glm::vec3(100.0f);
		m_ReflectPlane->m_transform->m_position = glm::vec3(0.0f, -1.0f, 0.0f);

		// raymarching
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
	}
}