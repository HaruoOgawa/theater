#include "MeshRendererComponent.h"
#include "./TransformComponent.h"
#include "GraphicsEngine/GraphicsMain/CTimeline.h"
#include "GraphicsEngine/Object/GameObject.h"
#include "GraphicsEngine/Graphics/GraphicsRenderer.h"

MeshRendererComponent::MeshRendererComponent(GameObject* o, PrimitiveType primType, RenderingSurfaceType SurfaceType,
	const std::string& vert, const std::string& frag, const std::string& geom, const std::string& tc, const std::string& tv, const std::string& cs)
	: m_mesh(nullptr), m_material(nullptr), myowner(o), useZTest(true), /*primTex(nullptr),*/ owner(o), game(GraphicsMain::GetInstance())
{
	useZTest = true;
	m_mesh = std::make_shared<Mesh>((primType));
	m_mesh->glDrawType = GLDrawType::NONE;
	m_material = std::make_shared<Material>(SurfaceType, vert, frag, geom, tc, tv,cs);
}

void MeshRendererComponent::Draw() {

	if (useZTest == true) {
		glEnable(GL_DEPTH_TEST);
	}
	else if (useZTest == false) {
		glDisable(GL_DEPTH_TEST);
	}

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	m_material->SetActive();
	m_material->SetMatrixUniform("MVPMatrix", owner->m_transform->m_pMatrix * owner->m_transform->m_vMatrix * owner->m_transform->m_mMatrix);
	m_material->SetMatrixUniform("MMatrix", owner->m_transform->m_mMatrix);
	m_material->SetMatrixUniform("VMatrix", owner->m_transform->m_vMatrix);
	m_material->SetMatrixUniform("PMatrix", owner->m_transform->m_pMatrix);
	m_material->SetFloatUniform("_time", game->time*0.001f);
	m_material->SetFloatUniform("_deltaTime", game->deltaTime);
	m_material->SetVec2Uniform("_resolution", GraphicsRenderer::GetInstance()->GetScreenSize());
	m_material->SetFloatUniform("_frameResolusion", GraphicsRenderer::GetInstance()->frameResolusion);
	if (GraphicsMain::GetInstance()->renderingTarget == ERerderingTarget::COLOR) {
		m_material->SetFloatUniform("_RenderingTarget", 1.0);
	}
	else if (GraphicsMain::GetInstance()->renderingTarget == ERerderingTarget::DEPTH) {
		m_material->SetFloatUniform("_RenderingTarget", 2.0);
	}

	//
	int CamIndex = GraphicsMain::GetInstance()->m_UseCameraIndex;
	if(
		(CamIndex >= 0 && CamIndex < GraphicsMain::GetInstance()->m_CameraTransformList.size())
		&& GraphicsMain::GetInstance()->m_CameraTransformList[CamIndex])
	{
		m_material->SetVec3Uniform("_WorldCameraPos", GraphicsMain::GetInstance()->m_CameraTransformList[CamIndex]->m_position);
	}

	// framebuffer
	GraphicsRenderer::GetInstance()->m_LatePostProcess_FrameTexture->SetActive(GL_TEXTURE0);
	m_material->SetTexUniform("frameTex", 0);
	
	for (auto clip : animationClips) {
		clip->callback(clip->lifeTimeRate);
	}

    m_mesh->Draw();

	//
	GraphicsRenderer::GetInstance()->m_LatePostProcess_FrameTexture->SetEnactive(GL_TEXTURE0);
}
