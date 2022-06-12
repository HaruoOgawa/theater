#include "CBoxInstancing.h"
#include "GraphicsEngine/Graphics/Mesh.h"
#include "GraphicsEngine/Graphics/Material.h"
#include "GraphicsEngine/Graphics/Graphics.h"
#include <GL/glew.h>
#include "GraphicsEngine/Component/TransformComponent.h"
#include "GraphicsEngine/Object/CameraObject.h"
#include "GraphicsEngine/Graphics/ComputeBuffer.h"
#include "GraphicsEngine/GraphicsMain/GraphicsMain.h"
#include "GraphicsEngine/Object/CNode.h"

CBoxInstancing::CBoxInstancing(Object* o)
	: Component(o),
	instanciedCound(124*124),
	sideCubeCount(124)
{
	Start();
}

CBoxInstancing::~CBoxInstancing() {
}

void CBoxInstancing::Start() {
	owner->GetRootNode()->GetComponent<TransformComponent>()->SetScale(glm::vec3(1.0));
	m_mesh = std::make_shared<Mesh>(PrimitiveType::CUBE);
	m_material = std::make_shared<Material>(
		"./Assets/Shader/Kura02/InstancedBox.vert",
		"./Assets/Shader/Kura02/InstancedBox.frag"
	);
	
	m_cubeGroundBuffer = std::make_shared<ComputeBuffer>(sizeof(SCubeFieldObj)*instanciedCound);
	std::vector<SCubeFieldObj> init_CubeFieldObj;
	for (int y = 0; y < sideCubeCount;y++) {
		for (int x = 0; x < sideCubeCount; x++) {
			init_CubeFieldObj.push_back(SCubeFieldObj(x, y, x + y * sideCubeCount));
		}
	}

	m_cubeGroundBuffer->SetData<std::vector<SCubeFieldObj>>(init_CubeFieldObj);

	//set buffer to material
	m_material->SetBuffer(m_cubeGroundBuffer, 0);
}

void CBoxInstancing::Update() {

}

void CBoxInstancing::Draw() {
	owner->GetRootNode()->GetComponent<TransformComponent>()->CalMatrix();
	m_material->SetActive();
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	m_material->SetMatrixUniform("MVPMatrix", owner->GetRootNode()->GetComponent<TransformComponent>()->GetMVPMatrix() ); //
	m_material->SetMatrixUniform("MMatrix", owner->GetRootNode()->GetComponent<TransformComponent>()->GetMMatrix());
	m_material->SetMatrixUniform("VMatrix", owner->GetRootNode()->GetComponent<TransformComponent>()->GetVMatrix());
	m_material->SetMatrixUniform("PMatrix", owner->GetRootNode()->GetComponent<TransformComponent>()->GetPMatrix());
	m_material->SetFloatUniform("_time", GraphicsMain::GetInstance()->time);
	m_material->SetFloatUniform("_deltaTime", GraphicsMain::GetInstance()->deltaTime);
	m_material->SetVec3Uniform("_CameraPosition", GraphicsMain::GetInstance()->game_camera_instance->GetPosition());
	m_material->SetIntUniform("_instancedCound", instanciedCound);
	m_material->SetIntUniform("_sideCubeCount", sideCubeCount);
	m_material->SetFloatUniform("_cubeWidth", 1.0);
	m_material->SetVec3Uniform("_lightDir", glm::vec3(-2.0, 1.0, 1.0));
	m_material->SetFloatUniform("_Roughness", 0.5);
	m_material->SetFloatUniform("_FresnelReflectance",1.0);
	Graphics::DrawInstancedWithMesh(m_mesh, instanciedCound, m_material,GL_TRIANGLES);
	
}

void CBoxInstancing::ProcessInput(const SDL_Event& e) {

}