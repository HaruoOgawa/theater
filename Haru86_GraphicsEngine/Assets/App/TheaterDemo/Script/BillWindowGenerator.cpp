#include "BillWindowGenerator.h"
#include "GraphicsEngine/Graphics/GraphicsRenderer.h"
#include "GraphicsEngine/Graphics/Texture.h"
#include "GraphicsEngine/Graphics/Mesh.h"
#include "GraphicsEngine/Graphics/Material.h"
#include "GraphicsEngine/Graphics/ShaderLib.h"

namespace myapp {
	BillWindowGenerator::BillWindowGenerator() :
		m_BillWindowTexIndex(0),
		m_BillWindowTex(std::make_shared<Texture>()),
		m_BillWindowMesh(nullptr),
		m_BillWindowMaterial(nullptr)
	{
		// Create Base Resource
		GraphicsRenderer::GetInstance()->CreateFrameBuffer(512, 512, m_BillWindowTex, m_BillWindowTexIndex, GL_RGBA, GL_RGBA, true);
		m_BillWindowMesh = std::make_shared<Mesh>(PrimitiveType::BOARD); // Œã‚ÅPOINT‚É‚·‚é
		std::string billWin_frag = {
			#include "../Shader/BillWindow.frag"
		};
		m_BillWindowMaterial = std::make_shared<Material>(RenderingSurfaceType::RASTERIZER, shaderlib::ShaderLib::StandardRenderBoard_vert, billWin_frag);

		// Draw Texture
		GraphicsMain::GetInstance()->renderingTarget = ERerderingTarget::COLOR;
		glBindFramebuffer(GL_FRAMEBUFFER,m_BillWindowTexIndex);
		glViewport(0, 0, static_cast<int>(GraphicsRenderer::GetInstance()->GetScreenSize().x * GraphicsRenderer::GetInstance()->frameResolusion),
			static_cast<int>(GraphicsRenderer::GetInstance()->GetScreenSize().y * GraphicsRenderer::GetInstance()->frameResolusion));
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glEnable(GL_DEPTH_TEST);

		m_BillWindowMaterial->SetActive();
		m_BillWindowMaterial->SetVec2Uniform("_resolution", GraphicsRenderer::GetInstance()->GetScreenSize());
		m_BillWindowMaterial->SetFloatUniform("_frameResolusion", GraphicsRenderer::GetInstance()->frameResolusion);
		
		m_BillWindowMesh->Draw();

		//glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
}