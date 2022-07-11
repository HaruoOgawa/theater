#include "PostProcess.h"
#include "GraphicsEngine/Graphics/Texture.h"
#include <glew.h>
#include "CBloom.h"
#include "GraphicsEngine/Graphics/Mesh.h"
#include "GraphicsEngine/Graphics/Material.h"
#include "GraphicsEngine/Graphics/GraphicsRenderer.h"
#include "GraphicsEngine/GraphicsMain/GraphicsMain.h"
#include "GraphicsEngine/Graphics/ShaderLib.h"
#include "GraphicsEngine/Component/TransformComponent.h"

//instanceを定義する
PostProcess* PostProcess::instance = nullptr;

void PostProcess::CreateInstance() {
	if (!instance)instance = new PostProcess();
}

PostProcess* PostProcess::GetInstance() {
	if (instance) return instance;
}

void PostProcess::DestroyInstance() {
	if (instance) {
		delete instance;
		instance = nullptr;
	}
}

PostProcess::PostProcess():
	m_UsePostProcess(false),
	m_UseBloom(false),
	m_BloomIntensity(0.0),
	m_Bloom(std::make_unique<CBloom>()),
	m_BloomTexture(std::make_shared<Texture>()),
	m_TRS(std::make_shared<TransformComponent>())
{
	m_mesh = std::make_shared<Mesh>(PrimitiveType::BOARD);
	m_material = std::make_shared<Material>(RenderingSurfaceType::RASTERIZER, shaderlib::ShaderLib::StandardRenderBoard_vert, shaderlib::ShaderLib::PolygonPostProcess_frag, "", "", "", "");
	m_LateMaterial = std::make_shared<Material>(RenderingSurfaceType::RASTERIZER, shaderlib::ShaderLib::StandardRenderBoard_vert, shaderlib::ShaderLib::LatePostProcess_frag, "", "", "", "");

	if (!GraphicsRenderer::GetInstance()->CreateFrameBuffer(GraphicsRenderer::GetInstance()->GetScreenSize().x, GraphicsRenderer::GetInstance()->GetScreenSize().y, m_BloomTexture, m_BloomFrameBuffer, GL_RGBA16F, GL_RGBA, GL_FLOAT)) {
		printf("Cannot Create FrameBuffer\n");
	}
}

void PostProcess::DrawPolygonPostProcess(const std::shared_ptr<Texture> SrcTexture, const unsigned int& DestBuffer)const {
	// Draw Bloom
	if (m_UseBloom)m_Bloom->Draw(SrcTexture, m_BloomFrameBuffer);

	// Draw PostProcess Result
	GraphicsMain::GetInstance()->renderingTarget = ERerderingTarget::COLOR;
	glBindFramebuffer(GL_FRAMEBUFFER, DestBuffer);
	glViewport(0, 0, static_cast<int>(GraphicsRenderer::GetInstance()->GetScreenSize().x * GraphicsRenderer::GetInstance()->frameResolusion), static_cast<int>(GraphicsRenderer::GetInstance()->GetScreenSize().y * GraphicsRenderer::GetInstance()->frameResolusion));

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glEnable(GL_DEPTH_TEST);

	// draw PostProcess Board
	m_material->SetActive();
	m_material->SetFloatUniform("_time", GraphicsMain::GetInstance()->time);
	m_material->SetVec2Uniform("_resolution", GraphicsRenderer::GetInstance()->GetScreenSize());
	m_material->SetFloatUniform("_frameResolusion", GraphicsRenderer::GetInstance()->frameResolusion);

	// Bloom
	m_BloomTexture->SetActive(GL_TEXTURE0);
	m_material->SetTexUniform("_BloomTexture", 0);
	m_material->SetFloatUniform("_UseBloom", (m_BloomTexture && m_UseBloom)? 1.0 : 0.0);
	

	// Set SrcTexture
	SrcTexture->SetActive(GL_TEXTURE1);
	m_material->SetTexUniform("_SrcTexture", 1);
	
	m_mesh->Draw();
	m_BloomTexture->SetEnactive(GL_TEXTURE0);
	SrcTexture->SetEnactive(GL_TEXTURE1);
}

// SSR
void PostProcess::DrawLatePostProcess(const std::shared_ptr<Texture> SrcTexture, const unsigned int& DestBuffer)const {
	// Draw PostProcess Result
	glBindFramebuffer(GL_FRAMEBUFFER, DestBuffer);
	//glViewport(0, 0, static_cast<int>(GraphicsRenderer::GetInstance()->GetScreenSize().x * GraphicsRenderer::GetInstance()->frameResolusion), static_cast<int>(GraphicsRenderer::GetInstance()->GetScreenSize().y * GraphicsRenderer::GetInstance()->frameResolusion));
	glViewport(0, 0, static_cast<int>(GraphicsRenderer::GetInstance()->GetScreenSize().x), static_cast<int>(GraphicsRenderer::GetInstance()->GetScreenSize().y));

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glEnable(GL_DEPTH_TEST);

	m_LateMaterial->SetActive();
	m_LateMaterial->SetFloatUniform("_time", GraphicsMain::GetInstance()->time);
	m_LateMaterial->SetVec2Uniform("_resolution", GraphicsRenderer::GetInstance()->GetScreenSize());
	m_LateMaterial->SetFloatUniform("_frameResolusion", GraphicsRenderer::GetInstance()->frameResolusion);
	m_LateMaterial->SetMatrixUniform("VPMatrix", m_TRS->m_pMatrix * m_TRS->m_vMatrix);
	m_LateMaterial->SetMatrixUniform("InvVPMatrix", glm::inverse(m_TRS->m_pMatrix * m_TRS->m_vMatrix));

	// カメラが定義されているなら情報を渡す
	if (GraphicsMain::GetInstance()->m_UsingCamera)
	{
		m_LateMaterial->SetVec3Uniform("_WorldCameraPos", GraphicsMain::GetInstance()->m_UsingCamera->m_position);
		m_LateMaterial->SetVec3Uniform("_WorldCameraCenter", GraphicsMain::GetInstance()->m_UsingCamera->m_center);
	}

	// テクスチャ
	SrcTexture->SetActive(GL_TEXTURE0);
	m_LateMaterial->SetTexUniform("_SrcTexture", 0);
	
	GraphicsRenderer::GetInstance()->polygon_normalTexture->SetActive(GL_TEXTURE1);
	m_LateMaterial->SetTexUniform("_NormalMap", 1);

	GraphicsRenderer::GetInstance()->polygon_depthTexture->SetActive(GL_TEXTURE2);
	m_LateMaterial->SetTexUniform("_DepthMapPolygone", 2);

	GraphicsRenderer::GetInstance()->raymarching_depthTexture->SetActive(GL_TEXTURE3);
	m_LateMaterial->SetTexUniform("_DepthMapRaymarch", 3);

	m_mesh->Draw();
	SrcTexture->SetEnactive(GL_TEXTURE0);
	GraphicsRenderer::GetInstance()->polygon_normalTexture->SetEnactive(GL_TEXTURE1);
	GraphicsRenderer::GetInstance()->polygon_depthTexture->SetEnactive(GL_TEXTURE2);
	GraphicsRenderer::GetInstance()->raymarching_depthTexture->SetEnactive(GL_TEXTURE3);
}
