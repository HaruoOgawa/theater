#include "MeshRendererComponent.h"
#include "./TransformComponent.h"
#include "GraphicsEngine/GraphicsMain/CTimeline.h"
#include "GraphicsEngine/Object/GameObject.h"

MeshRendererComponent::MeshRendererComponent(GameObject* o, PrimitiveType primType, RenderingSurfaceType SurfaceType,
	const std::string& vert, const std::string& frag, const std::string& geom, const std::string& tc, const std::string& tv, const std::string& cs)
	: m_mesh(nullptr), m_material(nullptr), myowner(o), useZTest(true), primTex(nullptr), owner(o), game(GraphicsMain::GetInstance())
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


	//ここでTextureをアクティブにする
	m_material->SetActiveTextureList();

	// framebuffer
	GraphicsRenderer::GetInstance()->m_LatePostProcess_FrameTexture->SetActive();
	m_material->SetTexUniform("frameTex", GraphicsRenderer::GetInstance()->m_LatePostProcess_FrameTexture->GetTextureID());
	GraphicsRenderer::GetInstance()->m_LatePostProcess_FrameTexture->SetEactive();

	for (auto clip : animationClips) {
		clip->callback(clip->lifeTimeRate);
	}

    m_mesh->Draw();

	//使い終わったらここでTextureを非アクティブにする
	m_material->SetEactiveTextureList();
}

void MeshRendererComponent::DrawInstancedWithMesh(std::shared_ptr<Mesh> mesh, int count, std::shared_ptr<Material> material, GLenum rendermode) {
	const auto& prim = mesh->m_primitives;
	for (int i = 0; i < prim.size(); i++) {
		prim[i]->SetActive();
		glDrawElementsInstanced(rendermode, prim[i]->mNumIndices, GL_UNSIGNED_SHORT, nullptr, count);
	}
}