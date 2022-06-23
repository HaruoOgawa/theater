#include "MeshRendererComponent.h"
#include <glew.h>
#include <glm/glm.hpp>
#include "./TransformComponent.h"
#include <string>
#include <iostream>
#include "../Graphics/GraphicsRenderer.h"
#include "GraphicsEngine/GraphicsMain/CTimeline.h"
#include "../Graphics/Texture.h"
#include "GraphicsEngine/Object/GameObject.h"

MeshRendererComponent::MeshRendererComponent(GameObject* o, PrimitiveType primType, RenderingSurfaceType SurfaceType,
	const std::string& vert, const std::string& frag, const std::string& geom, const std::string& tc, const std::string& tv)
	: Component(o), m_mesh(nullptr), m_material(nullptr), myowner(o), useZTest(true), primTex(nullptr)
{
	useZTest = true;
	m_mesh = std::make_shared<Mesh>((primType));
	m_mesh->glDrawType = GLDrawType::NONE;
	m_material = std::make_shared<Material>(SurfaceType, vert, frag, geom, tc, tv);
}

MeshRendererComponent::~MeshRendererComponent() {

}

void MeshRendererComponent::Update() {

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
	m_material->SetMatrixUniform("MVPMatrix", owner->m_transform->GetMVPMatrix());
	m_material->SetMatrixUniform("MMatrix", owner->m_transform->GetMMatrix());
	m_material->SetMatrixUniform("VMatrix", owner->m_transform->GetVMatrix());
	m_material->SetMatrixUniform("PMatrix", owner->m_transform->GetPMatrix());
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

	if (primTex != nullptr) {
		primTex->SetActive();
		m_material->SetTexUniform("_fontTexture", primTex->GetTextureID());
	}

	//ここでTextureをアクティブにする
	m_material->SetActiveTextureList();

	for (auto clip : animationClips) {
		clip->callback(clip->lifeTimeRate);
	}

	if (m_mesh->glDrawType == GLDrawType::TESSELLATION) {
		//glPatchParameteri(GL_PATCH_VERTICES, 4);
		m_mesh->Draw(m_mesh->glDrawType);
	}
	else {
		m_mesh->Draw();
	}

	//使い終わったらここでTextureを非アクティブにする
	m_material->SetEactiveTextureList();
}

void MeshRendererComponent::DrawBoard() {
	if (useZTest == true) {
		glEnable(GL_DEPTH_TEST);
	}
	else if (useZTest == false) {
		glDisable(GL_DEPTH_TEST);
	}

	m_material->SetActive();
	m_material->SetMatrixUniform("MVPMatrix", owner->m_transform->GetMVPMatrix());
	m_material->SetMatrixUniform("MMatrix", owner->m_transform->GetMMatrix());
	m_material->SetMatrixUniform("VMatrix", owner->m_transform->GetVMatrix());
	m_material->SetMatrixUniform("PMatrix", owner->m_transform->GetPMatrix());
	m_material->SetFloatUniform("_time", game->time);
	m_material->SetFloatUniform("_deltaTime", game->deltaTime);
	m_material->SetVec2Uniform("_resolution", GraphicsRenderer::GetInstance()->GetScreenSize());
	m_material->SetFloatUniform("_frameResolusion", GraphicsRenderer::GetInstance()->frameResolusion);

	// PolygonPostProcessのテスト
	/*if (GraphicsRenderer::GetInstance()->m_PolygonPostProcess_FrameTexture != nullptr) {
		GraphicsRenderer::GetInstance()->m_PolygonPostProcess_FrameTexture->SetActive();
		m_material->SetTexUniform("frameTex", GraphicsRenderer::GetInstance()->m_PolygonPostProcess_FrameTexture->GetTextureID());
		GraphicsRenderer::GetInstance()->m_PolygonPostProcess_FrameTexture->SetEactive();
	}*/

	// PostProcess後の最終結果 → m_LatePostProcess_FrameTexture
	if (GraphicsRenderer::GetInstance()->m_LatePostProcess_FrameTexture != nullptr) {
		GraphicsRenderer::GetInstance()->m_LatePostProcess_FrameTexture->SetActive();
		m_material->SetTexUniform("frameTex", GraphicsRenderer::GetInstance()->m_LatePostProcess_FrameTexture->GetTextureID());
		GraphicsRenderer::GetInstance()->m_LatePostProcess_FrameTexture->SetEactive();
	}

	for (auto clip : animationClips) {
		clip->callback(clip->lifeTimeRate);
	}

	m_mesh->Draw();
}

void MeshRendererComponent::DrawInstancedWithMesh(std::shared_ptr<Mesh> mesh, int count, std::shared_ptr<Material> material, GLenum rendermode) {
	const auto& prim = mesh->GetPrimitiveList();
	for (int i = 0; i < prim.size(); i++) {
		prim[i]->SetActive();
		glDrawElementsInstanced(rendermode, prim[i]->GetNumIndices(), GL_UNSIGNED_SHORT, nullptr, count);
	}
}

const std::shared_ptr<Mesh>& MeshRendererComponent::GetMesh()const {
	return m_mesh;
}

const std::shared_ptr<class Material>& MeshRendererComponent::GetMaterial()const {
	return m_material;
}

void MeshRendererComponent::SetUseZTest(bool use) {
	useZTest = use;
}

bool MeshRendererComponent::GetUseZTest()const {
	return useZTest;
}