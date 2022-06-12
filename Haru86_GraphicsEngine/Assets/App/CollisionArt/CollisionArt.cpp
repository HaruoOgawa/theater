#include "CollisionArt.h"
#include "GraphicsEngine/Object/TimelineObject.h"
#include "GraphicsEngine/Component/TimelineComponent.h"
#include "GraphicsEngine/Object/GameObject.h"
#include "GraphicsEngine/Object/Object.h"
#include "GraphicsEngine/Component/TransformComponent.h"
#include "GraphicsEngine/Object/CameraObject.h"
#include "GraphicsEngine/Component/CameraComponent.h"
#include <glm/glm.hpp>
#include "GraphicsEngine/GraphicsMain/GraphicsMain.h"
#include "./Script/CCollisionGPUBox.h"
#include "GraphicsEngine/Object/RaymarchingObject.h"
#include "GraphicsEngine/Graphics/PostProcess.h"
#include "Script/CKura3DGPUTrail.h"
#include "GraphicsEngine/Object/CNode.h"

CollisionArt::CollisionArt():
	BaseApp()
{

}

CollisionArt::~CollisionArt() {
	printf("CollisionArt is Released\n");
}

void CollisionArt::Start() {
	cam_obj = std::make_shared<CameraObject>(CameraType::FIXED_CAMERA);
	cam_obj->m_cameraComponent->cameraCenter = glm::vec3(0.0,-1.0,0.0);
	GraphicsMain::GetInstance()->game_camera_instance = cam_obj;
	cam_obj->GetRootNode()->GetComponent<TransformComponent>()->SetPosition(glm::vec3(8.5,0.0, 8.5));

	test_SphereObj = std::make_unique<GameObject>(PrimitiveType::SPHERE);
	test_SphereObj->SetPosition(glm::vec3(-2.0f, 5.0f, -2.0f));
	test_SphereObj->AddCollider(physics::EColliderType::Sphere,physics::EObjectType::DYNAMIC);
	
	test_SphereObj2 = std::make_unique<GameObject>(PrimitiveType::SPHERE);
	test_SphereObj2->SetPosition(glm::vec3(2.2f, 10.0f, 2.2f));
	test_SphereObj2->AddCollider(physics::EColliderType::Sphere,physics::EObjectType::DYNAMIC);

	test_CubeObj = std::make_unique<GameObject>(PrimitiveType::CUBE, "./Assets/Shader/Standard.vert","./Assets/Shader/CollisionArt/PlaneMarker.frag");
	test_CubeObj->SetPosition(glm::vec3(0.0f,-5.0f,0.0));
	test_CubeObj->SetScale(glm::vec3(50.0f, 0.5f, 50.0f));
	test_CubeObj->AddCollider(physics::EColliderType::Box,physics::EObjectType::STATIC);

	// ポストプロセスの使用を宣言
	PostProcess::GetInstance()->EnablePostProcess();
	PostProcess::GetInstance()->EnableBloom(1.0,1.0);
}

void CollisionArt::Update() {
	if (GraphicsMain::GetInstance()->time >= 1000.0f * 5.0f) {
		GraphicsMain::GetInstance()->Restart();
	}
}

void CollisionArt::Draw() {
}

void CollisionArt::Timeline(class TimelineObject* timelineObj) {
}