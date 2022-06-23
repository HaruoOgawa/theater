#include "RaymarchingObject.h"
#include "GraphicsEngine/Graphics/Material.h"
#include "GraphicsEngine/Graphics/Mesh.h"
#include "GraphicsEngine/GraphicsMain/GraphicsMain.h"
#include "GraphicsEngine/Component/TransformComponent.h"
#include "GraphicsEngine/Component/TransformComponent.h"
#include "GraphicsEngine/Graphics/ShaderLib.h"

RaymarchingObject::RaymarchingObject(const std::string& fragmentShaderCode)
	: Object()
{
	m_material = std::make_shared<Material>(RenderingSurfaceType::RAYMARCHING, shaderlib::ShaderLib::RaymarchingObject_vert, fragmentShaderCode, "", "", "");
	//m_material = std::make_shared<Material>("./Assets/Shader/RaymarchingObject.vert",fragmentShaderName,RenderingSurfaceType::RAYMARCHING);
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
	m_material->SetMatrixUniform("MVPMatrix", m_transform->GetMVPMatrix());
	m_material->SetMatrixUniform("MMatrix", m_transform->GetMMatrix());
	m_material->SetMatrixUniform("VMatrix", m_transform->GetVMatrix());
	m_material->SetMatrixUniform("PMatrix", m_transform->GetPMatrix());
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
	
	m_mesh->Draw();
}