#include "PolygonRaymarchingMixer.h"
#include "GraphicsEngine/Graphics/Material.h"
#include "GraphicsEngine/Graphics/Mesh.h"
#include "GraphicsEngine/Graphics/GraphicsRenderer.h"
#include "GraphicsEngine/Graphics/Texture.h"
#include "GraphicsEngine/Graphics/ShaderLib.h"

PolygonRaymarchingMixer::PolygonRaymarchingMixer() {
	m_material = std::make_unique<Material>(RenderingSurfaceType::RASTERIZER, shaderlib::ShaderLib::PolygonRaymarchingMixer_vert, shaderlib::ShaderLib::PolygonRaymarchingMixer_frag, "", "", "");
	m_mesh = std::make_unique<Mesh>(PrimitiveType::BOARD);
}

PolygonRaymarchingMixer::~PolygonRaymarchingMixer() {

}

void PolygonRaymarchingMixer::Draw() {
	m_material->SetActive();
	m_material->SetFloatUniform("_frameResolusion", GraphicsRenderer::GetInstance()->frameResolusion);
	//m_PolygonPostProcess_FrameTexture
	if (GraphicsRenderer::GetInstance()->m_PolygonPostProcess_FrameTexture != nullptr) {
		GraphicsRenderer::GetInstance()->m_PolygonPostProcess_FrameTexture->SetActive();
		m_material->SetTexUniform("polygon_frameTexture", GraphicsRenderer::GetInstance()->m_PolygonPostProcess_FrameTexture->GetTextureID());
		GraphicsRenderer::GetInstance()->m_PolygonPostProcess_FrameTexture->SetEactive();
	}
	//polygon_depthTexture
	if (GraphicsRenderer::GetInstance()->polygon_depthTexture != nullptr) {
		GraphicsRenderer::GetInstance()->polygon_depthTexture->SetActive();
		m_material->SetTexUniform("polygon_depthTexture", GraphicsRenderer::GetInstance()->polygon_depthTexture->GetTextureID());
		GraphicsRenderer::GetInstance()->polygon_depthTexture->SetEactive();
	}

	if (GraphicsMain::GetInstance()->raymarchingObjectList.size() > 0) {
		//raymarching_frameTexture
		if (GraphicsRenderer::GetInstance()->raymarching_frameTexture != nullptr) {
			GraphicsRenderer::GetInstance()->raymarching_frameTexture->SetActive();
			m_material->SetTexUniform("raymarching_frameTexture", GraphicsRenderer::GetInstance()->raymarching_frameTexture->GetTextureID());
			GraphicsRenderer::GetInstance()->raymarching_frameTexture->SetEactive();
		}
		//raymarching_depthTexture
		if (GraphicsRenderer::GetInstance()->raymarching_depthTexture != nullptr) {
			GraphicsRenderer::GetInstance()->raymarching_depthTexture->SetActive();
			m_material->SetTexUniform("raymarching_depthTexture", GraphicsRenderer::GetInstance()->raymarching_depthTexture->GetTextureID());
			GraphicsRenderer::GetInstance()->raymarching_depthTexture->SetEactive();
		}
	}
	else {
		m_material->SetFloatUniform("_existRaymarching", 1.0);
	}
	
	m_mesh->Draw();
}