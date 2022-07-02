#include "ProceduralCity.h"
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
	ProceduralCity::ProceduralCity():
		m_transform(std::make_shared<TransformComponent>())
	{
		Start();
	}

	void ProceduralCity::Start() 
	{	
		//
		m_BillMesh4Instanced = std::make_shared<Mesh>(PrimitiveType::POINT);
		std::string ProceduralCity_vert = {
			#include "../Shader/ProceduralCity.vert"
		};

		std::string ProceduralCity_frag = {
			#include "../Shader/ProceduralCity.frag"
		};

		std::string ProceduralCity_geom = {
			#include "../Shader/ProceduralCity.geom"
		};
		m_BillMaterial4Instanced = std::make_shared<Material>(RenderingSurfaceType::RASTERIZER, ProceduralCity_vert, ProceduralCity_frag, ProceduralCity_geom);

		//
		m_BillWindowGenerator = std::make_shared<BillWindowGenerator>();
		
		// raymarching
		std::string MandelboxShader = {
			#include "../Shader/MandelboxSample.frag"
		};

		m_Mandelbox = std::make_shared<GameObject>(
			PrimitiveType::BOARD,
			RenderType::DefaultBuffer,
			RenderQueue::Geometry,
			RenderingSurfaceType::RAYMARCHING,
			shaderlib::ShaderLib::RaymarchingObject_vert,
			MandelboxShader
			);
	}

	void ProceduralCity::Update() 
	{
	}

	void ProceduralCity::Draw() {
		//Console::Log("m_BillWindowGenerator->m_BillWindowTex->GetTextureID(): %d\n", m_BillWindowGenerator->m_BillWindowTex->GetTextureID());

		m_BillMaterial4Instanced->SetActive();
		m_transform->CalMatrix();
		m_BillMaterial4Instanced->SetMatrixUniform("MVPMatrix", m_transform->m_pMatrix * m_transform->m_vMatrix * m_transform->m_mMatrix);
		m_BillMaterial4Instanced->SetMatrixUniform("MMatrix", m_transform->m_mMatrix);
		m_BillMaterial4Instanced->SetMatrixUniform("VMatrix", m_transform->m_vMatrix);
		m_BillMaterial4Instanced->SetMatrixUniform("PMatrix", m_transform->m_pMatrix);
		m_BillMaterial4Instanced->SetVec2Uniform("_resolution", GraphicsRenderer::GetInstance()->GetScreenSize());
		m_BillMaterial4Instanced->SetFloatUniform("_frameResolusion", GraphicsRenderer::GetInstance()->frameResolusion);
		m_BillWindowGenerator->m_BillWindowTex->SetActive();
		m_BillMaterial4Instanced->SetTexUniform("_BillWindowTex", m_BillWindowGenerator->m_BillWindowTex->GetTextureID());
		m_BillWindowGenerator->m_BillWindowTex->SetEactive();

		m_BillMesh4Instanced->DrawInstancedWithMesh(1024, GL_POINTS);
		//m_BillMesh4Instanced->Draw(GL_POINTS);
	}
}