#include "RaymarchingObject.h"
#include "GraphicsEngine/Graphics/Material.h"
#include "GraphicsEngine/Graphics/Mesh.h"
#include "GraphicsEngine/GraphicsMain/GraphicsMain.h"
#include "GraphicsEngine/Component/TransformComponent.h"
#include "GraphicsEngine/Object/CameraObject.h"
#include "GraphicsEngine/Component/TransformComponent.h"
#include "GraphicsEngine/Object/CNode.h"

RaymarchingObject::RaymarchingObject(std::string fragmentShaderName)
	: Object()
{
	m_material = std::make_shared<Material>("./Assets/Shader/RaymarchingObject.vert",fragmentShaderName,RenderingSurfaceType::RAYMARCHING);
	m_mesh = std::make_shared<Mesh>(PrimitiveType::BOARD);
}

RaymarchingObject::~RaymarchingObject() {

}

void RaymarchingObject::Start() {

}

void RaymarchingObject::Update() {

}

void RaymarchingObject::Draw() {
	m_material->SetActive();
	m_material->SetMatrixUniform("MVPMatrix", GetRootNode()->GetTransform()->GetMVPMatrix());
	m_material->SetMatrixUniform("MMatrix", GetRootNode()->GetTransform()->GetMMatrix());
	m_material->SetMatrixUniform("VMatrix", GetRootNode()->GetTransform()->GetVMatrix());
	m_material->SetMatrixUniform("PMatrix", GetRootNode()->GetTransform()->GetPMatrix());
	m_material->SetFloatUniform("_time", GraphicsMain::GetInstance()->time*0.001);
	m_material->SetFloatUniform("_deltaTime", GraphicsMain::GetInstance()->deltaTime);
	m_material->SetVec2Uniform("_resolution", GraphicsRenderer::GetInstance()->GetScreenSize());
	m_material->SetFloatUniform("_frameResolusion", GraphicsRenderer::GetInstance()->frameResolusion);
	if (GraphicsMain::GetInstance()->renderingTarget==ERerderingTarget::COLOR) {
		m_material->SetFloatUniform("_RenderingTarget", 1.0);
	}
	else if (GraphicsMain::GetInstance()->renderingTarget == ERerderingTarget::DEPTH) {
		m_material->SetFloatUniform("_RenderingTarget", 2.0);
	}
	m_material->SetVec3Uniform("_camerePosition", GraphicsMain::GetInstance()->game_camera_instance->GetRootNode()->GetComponent<TransformComponent>()->GetPosition());

	m_mesh->Draw();
}