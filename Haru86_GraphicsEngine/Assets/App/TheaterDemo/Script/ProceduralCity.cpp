#include "ProceduralCity.h"
#include "GraphicsEngine/Graphics/GraphicsRenderer.h"
#include "GraphicsEngine/Graphics/Texture.h"
#include "GraphicsEngine/Graphics/Mesh.h"
#include "GraphicsEngine/Graphics/Material.h"
#include "GraphicsEngine/Graphics/ShaderLib.h"
#include "GraphicsEngine/Component/TransformComponent.h"

namespace myapp {
	ProceduralCity::ProceduralCity():
		m_BillTexIndex(0),
		m_BillTex(std::make_shared<Texture>()),
		m_transform(std::make_shared<TransformComponent>())
	{
		//
		GraphicsRenderer::GetInstance()->CreateFrameBuffer(512, 512, m_BillTex, m_BillTexIndex, GL_RGBA, GL_RGBA, true);
		m_BillMesh4Instanced = std::make_shared<Mesh>(PrimitiveType::BOARD); // Œã‚ÅPOINT‚É‚·‚é
		std::string billWin_frag = {
			#include "../Shader/BillWindow.frag"
		};
		m_BillMaterial4Instanced = std::make_shared<Material>(RenderingSurfaceType::RASTERIZER, shaderlib::ShaderLib::Standard_vert, billWin_frag);
	}

	void ProceduralCity::Start() 
	{	
	}

	void ProceduralCity::Update() 
	{
	}

	void ProceduralCity::Draw() {

		m_BillMaterial4Instanced->SetActive();
		m_transform->CalMatrix();
		m_BillMaterial4Instanced->SetMatrixUniform("MVPMatrix", m_transform->m_pMatrix * m_transform->m_vMatrix * m_transform->m_mMatrix);
		m_BillMaterial4Instanced->SetMatrixUniform("MMatrix", m_transform->m_mMatrix);
		m_BillMaterial4Instanced->SetMatrixUniform("VMatrix", m_transform->m_vMatrix);
		m_BillMaterial4Instanced->SetMatrixUniform("PMatrix", m_transform->m_pMatrix);
		m_BillMaterial4Instanced->SetVec2Uniform("_resolution", GraphicsRenderer::GetInstance()->GetScreenSize());
		m_BillMaterial4Instanced->SetFloatUniform("_frameResolusion", GraphicsRenderer::GetInstance()->frameResolusion);
		
		m_BillMesh4Instanced->Draw();
	}
}