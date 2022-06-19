#include "MeshRendererComponent.h"
#include <glew.h>
#include <glm/glm.hpp>
#include "./TransformComponent.h"
#include <string>
#include <iostream>
#include "../Graphics/GraphicsRenderer.h"
#include "./TimelineComponent.h"
#include "../Graphics/Texture.h"
#include "../Graphics/Font.h"
#include "GraphicsEngine/Object/CameraObject.h"
#include "GraphicsEngine/Object/CNode.h"

MeshRendererComponent::MeshRendererComponent(Object* o, PrimitiveType primType,
	const std::string& vert, const std::string& frag, const std::string& geom, const std::string& tc, const std::string& tv)
	: ARendererComponent(o), primTex(nullptr)
{
	useZTest = true;
	mesh = std::make_shared<Mesh>((primType));
	mesh->glDrawType = GLDrawType::NONE;
	material = std::make_shared<Material>(vert, frag, geom, tc, tv);
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

	material->SetActive();
	material->SetMatrixUniform("MVPMatrix", owner->GetRootNode()->GetComponent<TransformComponent>()->GetMVPMatrix());
	material->SetMatrixUniform("MMatrix", owner->GetRootNode()->GetComponent<TransformComponent>()->GetMMatrix());
	material->SetMatrixUniform("VMatrix", owner->GetRootNode()->GetComponent<TransformComponent>()->GetVMatrix());
	material->SetMatrixUniform("PMatrix", owner->GetRootNode()->GetComponent<TransformComponent>()->GetPMatrix());
	material->SetFloatUniform("_time", game->time);
	material->SetFloatUniform("_deltaTime", game->deltaTime);
	material->SetVec2Uniform("_resolution", GraphicsRenderer::GetInstance()->GetScreenSize());
	material->SetVec3Uniform("_CameraPos", GraphicsMain::GetInstance()->game_camera_instance->GetPosition());

	if (primTex != nullptr) {
		primTex->SetActive();
		material->SetTexUniform("_fontTexture", primTex->GetTextureID());
	}

	//ここでTextureをアクティブにする
	material->SetActiveTextureList();

	for (auto clip : animationClips) {
		clip->callback(clip->lifeTimeRate);
	}

	if (mesh->glDrawType == GLDrawType::TESSELLATION) {
		//glPatchParameteri(GL_PATCH_VERTICES, 4);
		mesh->Draw(mesh->glDrawType);
	}
	else {
		mesh->Draw();
	}

	//使い終わったらここでTextureを非アクティブにする
	material->SetEactiveTextureList();
}

void MeshRendererComponent::DrawBoard() {
	if (useZTest == true) {
		glEnable(GL_DEPTH_TEST);
	}
	else if (useZTest == false) {
		glDisable(GL_DEPTH_TEST);
	}

	material->SetActive();
	material->SetMatrixUniform("MVPMatrix", owner->GetRootNode()->GetComponent<TransformComponent>()->GetMVPMatrix());
	material->SetMatrixUniform("MMatrix", owner->GetRootNode()->GetComponent<TransformComponent>()->GetMMatrix());
	material->SetMatrixUniform("VMatrix", owner->GetRootNode()->GetComponent<TransformComponent>()->GetVMatrix());
	material->SetMatrixUniform("PMatrix", owner->GetRootNode()->GetComponent<TransformComponent>()->GetPMatrix());
	material->SetFloatUniform("_time", game->time);
	material->SetFloatUniform("_deltaTime", game->deltaTime);
	material->SetVec2Uniform("_resolution", GraphicsRenderer::GetInstance()->GetScreenSize());
	material->SetFloatUniform("_frameResolusion", GraphicsRenderer::GetInstance()->frameResolusion);

	// PolygonPostProcessのテスト
	/*if (GraphicsRenderer::GetInstance()->m_PolygonPostProcess_FrameTexture != nullptr) {
		GraphicsRenderer::GetInstance()->m_PolygonPostProcess_FrameTexture->SetActive();
		material->SetTexUniform("frameTex", GraphicsRenderer::GetInstance()->m_PolygonPostProcess_FrameTexture->GetTextureID());
		GraphicsRenderer::GetInstance()->m_PolygonPostProcess_FrameTexture->SetEactive();
	}*/

	// PostProcess後の最終結果 → m_LatePostProcess_FrameTexture
	if (GraphicsRenderer::GetInstance()->m_LatePostProcess_FrameTexture != nullptr) {
		GraphicsRenderer::GetInstance()->m_LatePostProcess_FrameTexture->SetActive();
		material->SetTexUniform("frameTex", GraphicsRenderer::GetInstance()->m_LatePostProcess_FrameTexture->GetTextureID());
		GraphicsRenderer::GetInstance()->m_LatePostProcess_FrameTexture->SetEactive();
	}

	for (auto clip : animationClips) {
		clip->callback(clip->lifeTimeRate);
	}

	mesh->Draw();
}



void MeshRendererComponent::ProcessInput(const std::shared_ptr<app::CEventListener>& EventListener) {

}
