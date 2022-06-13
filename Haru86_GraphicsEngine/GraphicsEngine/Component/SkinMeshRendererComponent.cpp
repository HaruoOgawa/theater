#include "SkinMeshRendererComponent.h"
#include "GraphicsEngine/Object/Object.h"
#include "GraphicsEngine/Object/CNode.h"
#include "GraphicsEngine/Component/Component.h"
#include "GraphicsEngine/Component/TransformComponent.h"
#include "GraphicsEngine/Graphics/Mesh.h"
#include "GraphicsEngine/Graphics/Material.h"
#include "GraphicsEngine/GraphicsMain/GraphicsMain.h"
#include "GraphicsEngine/Object/CameraObject.h"
#include "GraphicsEngine/Animation/Skeleton.h"
#include "GraphicsEngine/Animation/Pose.h"
#include "GraphicsEngine/Message/Console.h"
#include "GraphicsEngine/Animation/AnimatorComponent.h"

SkinMeshRendererComponent::SkinMeshRendererComponent(Object* o, std::string objPath, const std::shared_ptr<Mesh>& MeshData, const std::shared_ptr<animation::AnimatorComponent>& Animator,
	int MeshIndex, const std::string& NodeName, std::string vertexShaderName, std::string fragmentShaderName) :
	ARendererComponent(o),
	m_MeshIndex(MeshIndex),
	m_NodeName(NodeName)
{
	mesh = MeshData;
	m_Animator = Animator;
	useZTest = true;
	mesh->glDrawType = GLDrawType::NONE;
	material = std::make_shared<Material>(vertexShaderName, fragmentShaderName);
}

SkinMeshRendererComponent::~SkinMeshRendererComponent() 
{
}

void SkinMeshRendererComponent::Update() 
{
}

void SkinMeshRendererComponent::Draw() {
}

void SkinMeshRendererComponent::Draw(const std::vector<glm::mat4>& ModelMatrixTree) {
	glm::mat4 MMatrix = glm::mat4(1.0);
	for (int MatIndex = 0; MatIndex < ModelMatrixTree.size(); MatIndex++) {
	//for (int MatIndex = ModelMatrixTree.size() - 1; MatIndex >= 0; MatIndex--) {
		const auto& mat = ModelMatrixTree[MatIndex];
		MMatrix *= mat;
	}
	
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
	material->SetMatrixUniform("MMatrix", MMatrix);
	material->SetMatrixUniform("VMatrix", owner->GetRootNode()->GetComponent<TransformComponent>()->GetVMatrix());
	material->SetMatrixUniform("PMatrix", owner->GetRootNode()->GetComponent<TransformComponent>()->GetPMatrix());
	material->SetFloatUniform("_time", game->time);
	material->SetFloatUniform("_deltaTime", game->deltaTime);
	material->SetVec2Uniform("_resolution", GraphicsRenderer::GetInstance()->GetScreenSize());
	material->SetVec3Uniform("_CameraPos", GraphicsMain::GetInstance()->game_camera_instance->GetPosition());

	// skin mesh uniform
	if (m_Animator) {
		material->SetMatrixVectorUniform("pose", m_Animator->GetPoseMatrix());
		material->SetMatrixVectorUniform("invBindPose", m_Animator->GetSkeleton()->GetInvBindPose());
		material->SetFloatUniform("_MaxBoneNum", static_cast<unsigned int>(m_Animator->GetPoseMatrix().size()));
	}

	//
	mesh->Draw();
}

void SkinMeshRendererComponent::ProcessInput(const std::shared_ptr<app::CEventListener>& EventListener) {

}