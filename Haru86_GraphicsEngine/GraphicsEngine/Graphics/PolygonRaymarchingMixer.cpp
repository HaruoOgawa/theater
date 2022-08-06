#include "PolygonRaymarchingMixer.h"
#include "GraphicsEngine/Graphics/Material.h"
#include "GraphicsEngine/Graphics/Mesh.h"
#include "GraphicsEngine/Graphics/GraphicsRenderer.h"
#include "GraphicsEngine/Graphics/Texture.h"
#include "GraphicsEngine/Graphics/ShaderLib.h"

PolygonRaymarchingMixer::PolygonRaymarchingMixer() {
	m_material = std::make_unique<Material>(RenderingSurfaceType::RASTERIZER, shaderlib::ShaderLib::PolygonRaymarchingMixer_vert, shaderlib::ShaderLib::PolygonRaymarchingMixer_frag, "", "", "", "");
	m_mesh = std::make_unique<Mesh>(PrimitiveType::BOARD);
}

PolygonRaymarchingMixer::~PolygonRaymarchingMixer() {

}

void PolygonRaymarchingMixer::Draw(bool IsDepthMix) {
	// 基本パラメーター
	m_material->SetActive();
	m_material->SetFloatUniform("_frameResolusion", GraphicsRenderer::GetInstance()->frameResolusion);
	m_material->SetVec2Uniform("_resolution", GraphicsRenderer::GetInstance()->GetScreenSize());
	m_material->SetFloatUniform("_time", GraphicsMain::GetInstance()->time * 0.001f);

	// デプスマップをミックスするかどうか
	if (IsDepthMix) 
	{
		m_material->SetFloatUniform("_IsDepthMix", 1.0);
	}
	else 
	{
		m_material->SetFloatUniform("_IsDepthMix", 0.0);
	}

	//m_PolygonPostProcess_FrameTexture
	if (GraphicsRenderer::GetInstance()->m_PolygonPostProcess_FrameTexture != nullptr) {
		GraphicsRenderer::GetInstance()->m_PolygonPostProcess_FrameTexture->SetActive(GL_TEXTURE0);
		m_material->SetTexUniform("polygon_frameTexture", 0);
		
	}
	//polygon_depthTexture
	if (GraphicsRenderer::GetInstance()->polygon_depthTexture != nullptr) {
		GraphicsRenderer::GetInstance()->polygon_depthTexture->SetActive(GL_TEXTURE1);
		m_material->SetTexUniform("polygon_depthTexture", 1);
		
	}
	
	//if (GraphicsMain::GetInstance()->raymarchingObjectList.size() > 0) 
	{
		//raymarching_frameTexture
		if (GraphicsRenderer::GetInstance()->raymarching_frameTexture != nullptr) {
			GraphicsRenderer::GetInstance()->raymarching_frameTexture->SetActive(GL_TEXTURE2);
			m_material->SetTexUniform("raymarching_frameTexture", 2);
			
		}
		//raymarching_depthTexture
		if (GraphicsRenderer::GetInstance()->raymarching_depthTexture != nullptr) {
			GraphicsRenderer::GetInstance()->raymarching_depthTexture->SetActive(GL_TEXTURE3);
			m_material->SetTexUniform("raymarching_depthTexture", 3);
			
		}
	}
	/*else {
		m_material->SetFloatUniform("_existRaymarching", 1.0);
	}*/
	
	//polygon_normalTexture
	if (GraphicsRenderer::GetInstance()->polygon_normalTexture != nullptr) {
		GraphicsRenderer::GetInstance()->polygon_normalTexture->SetActive(GL_TEXTURE4);
		m_material->SetTexUniform("polygon_normalTexture", 4);

	}

	m_mesh->Draw();
	GraphicsRenderer::GetInstance()->m_PolygonPostProcess_FrameTexture->SetEnactive(GL_TEXTURE0);
	GraphicsRenderer::GetInstance()->polygon_depthTexture->SetEnactive(GL_TEXTURE1);
	GraphicsRenderer::GetInstance()->raymarching_frameTexture->SetEnactive(GL_TEXTURE2);
	GraphicsRenderer::GetInstance()->raymarching_depthTexture->SetEnactive(GL_TEXTURE3);
	GraphicsRenderer::GetInstance()->polygon_normalTexture->SetEnactive(GL_TEXTURE4);
}