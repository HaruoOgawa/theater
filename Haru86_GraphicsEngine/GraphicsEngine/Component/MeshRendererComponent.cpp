#include "MeshRendererComponent.h"
#include "./TransformComponent.h"
#include "GraphicsEngine/GraphicsMain/CTimeline.h"
#include "GraphicsEngine/Object/GameObject.h"
#include "GraphicsEngine/Graphics/GraphicsRenderer.h"

MeshRendererComponent::MeshRendererComponent(GameObject* o, PrimitiveType primType, RenderingSurfaceType SurfaceType,
	const std::string& vert, const std::string& frag, const std::string& geom,
	const std::string& tc, const std::string& tv, const std::string& cs, std::function<void(void)> calllback) : 
	m_mesh(nullptr), 
	m_material(nullptr), 
	myowner(o), 
	useZTest(true), 
	owner(o), 
	game(GraphicsMain::GetInstance()), 
	m_calllback(calllback)
{
	m_SurfaceType = SurfaceType;
	m_mesh = std::make_shared<Mesh>((primType));
	m_mesh->glDrawType = GLDrawType::NONE;
	m_material = std::make_shared<Material>(SurfaceType, vert, frag, geom, tc, tv,cs);

	// アルファブレンドの初期値
	if (m_SurfaceType==RenderingSurfaceType::RASTERIZER) {
		useAlphaTest = true;
	}
	else {
		useAlphaTest = false;
	}
}

void MeshRendererComponent::Draw() {

	if (useZTest == true) {
		glEnable(GL_DEPTH_TEST);
	}
	else if (useZTest == false) {
		glDisable(GL_DEPTH_TEST);
	}

	if (useAlphaTest) {
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}
	else 
	{
		glDisable(GL_BLEND);
	}

	m_material->SetActive();
	m_material->SetMatrixUniform("MVPMatrix", owner->m_transform->m_pMatrix * owner->m_transform->m_vMatrix * owner->m_transform->m_mMatrix);
	m_material->SetMatrixUniform("MMatrix", owner->m_transform->m_mMatrix);
	m_material->SetMatrixUniform("VMatrix", owner->m_transform->m_vMatrix);
	m_material->SetMatrixUniform("PMatrix", owner->m_transform->m_pMatrix);
	m_material->SetMatrixUniform("VPMatrix", owner->m_transform->m_pMatrix * owner->m_transform->m_vMatrix);
	m_material->SetMatrixUniform("InvVPMatrix", glm::inverse(owner->m_transform->m_pMatrix * owner->m_transform->m_vMatrix));
	m_material->SetFloatUniform("_time", game->time*0.001f);
	m_material->SetFloatUniform("_deltaTime", game->deltaTime);
	m_material->SetVec2Uniform("_resolution", GraphicsRenderer::GetInstance()->GetScreenSize());
	m_material->SetFloatUniform("_frameResolusion", GraphicsRenderer::GetInstance()->frameResolusion);

	// レイマーチングのデプスマップ用
	if (GraphicsMain::GetInstance()->renderingTarget == ERerderingTarget::COLOR) {
		m_material->SetFloatUniform("_RenderingTarget", 1.0);
	}
	else if (GraphicsMain::GetInstance()->renderingTarget == ERerderingTarget::DEPTH) {
		m_material->SetFloatUniform("_RenderingTarget", 2.0);
	}

	// カメラが定義されているなら情報を渡す
	if(GraphicsMain::GetInstance()->m_UsingCamera)
	{
		m_material->SetVec3Uniform("_WorldCameraPos", GraphicsMain::GetInstance()->m_UsingCamera->m_position);
		m_material->SetVec3Uniform("_WorldCameraCenter", GraphicsMain::GetInstance()->m_UsingCamera->m_center);
	}

	// framebuffer
	GraphicsRenderer::GetInstance()->m_LatePostProcess_FrameTexture->SetActive(GL_TEXTURE0);
	m_material->SetTexUniform("frameTex", 0);
	
	for (auto clip : animationClips) {
		clip->callback(clip->lifeTimeRate);
	}

	//
	m_calllback();

    m_mesh->Draw();

	//
	GraphicsRenderer::GetInstance()->m_LatePostProcess_FrameTexture->SetEnactive(GL_TEXTURE0);
}
