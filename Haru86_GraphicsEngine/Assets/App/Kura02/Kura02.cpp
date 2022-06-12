#include "Kura02.h"
#include "GraphicsEngine/Object/TimelineObject.h"
#include "GraphicsEngine/Component/TimelineComponent.h"
#include "GraphicsEngine/Object/GameObject.h"
#include "GraphicsEngine/Object/Object.h"
#include "GraphicsEngine/Component/TransformComponent.h"
#include "GraphicsEngine/Object/CameraObject.h"
#include "GraphicsEngine/Component/CameraComponent.h"
#include <glm/glm.hpp>
#include "GraphicsEngine/GraphicsMain/GraphicsMain.h"
#include "./Script/CBoxInstancing.h"
#include "GraphicsEngine/Object/RaymarchingObject.h"
#include "GraphicsEngine/Graphics/PostProcess.h"
#include "Script/CKura3DGPUTrail.h"
#include "GraphicsEngine/Object/CNode.h"

Kura02::Kura02():
	BaseApp()
{

}

Kura02::~Kura02() {
	printf("Kura02 is Released\n");
}

void Kura02::Start() {
	cam_obj = std::make_shared<CameraObject>(CameraType::FIXED_CAMERA);
	//cam_obj->SetPosition(glm::vec3(0.0, 0.0, -3.0));
	cam_obj->m_cameraComponent->cameraCenter = glm::vec3(0.0,-1.0,0.0);
	GraphicsMain::GetInstance()->game_camera_instance = cam_obj;
	cam_obj->GetRootNode()->GetComponent<TransformComponent>()->SetPosition(glm::vec3(5.0,0.0,5.0));

	ins_cubes = std::make_unique<Object>();
	ins_cubes->GetRootNode()->AddComponent<CBoxInstancing>(std::make_shared<CBoxInstancing>(ins_cubes.get()));
	ins_cubes->GetRootNode()->AddComponent<CKura3DGPUTrail>(std::make_shared<CKura3DGPUTrail>(ins_cubes.get()));

	raymarching_obj = std::make_unique<RaymarchingObject>("./Assets/Shader/Kura02/CubeFieldRaymarching.frag");
	GraphicsMain::GetInstance()->m_RaymarchingObject = raymarching_obj.get();

	// ポストプロセスの使用を宣言
	PostProcess::GetInstance()->EnablePostProcess();
	PostProcess::GetInstance()->EnableBloom(1.0,1.0);
}

void Kura02::Update() {
	ins_cubes->Update();
	cam_obj->GetRootNode()->GetComponent<TransformComponent>()->SetPosition(glm::vec3(
		glm::cos(GraphicsMain::GetInstance()->time * 0.0001)*8.5,
		0.0, 
		glm::sin(GraphicsMain::GetInstance()->time * 0.0001)* 8.5
	));

	if (GraphicsMain::GetInstance()->time>=1000.0*60.0*60.0) {
		GraphicsMain::GetInstance()->Restart();
	}
}

void Kura02::Draw() {
	ins_cubes->GetRootNode()->GetComponent<CBoxInstancing>()->Draw();
	ins_cubes->GetRootNode()->GetComponent<CKura3DGPUTrail>()->Draw();
}

void Kura02::Timeline(class TimelineObject* timelineObj) {

}