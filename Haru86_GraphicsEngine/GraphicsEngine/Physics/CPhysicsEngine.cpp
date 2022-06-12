#include "CPhysicsEngine.h"
#include "CColliderTree.h"
#include "GraphicsEngine/Object/GameObject.h"
#include "ACollider.h"
#include "GraphicsEngine/Component/TransformComponent.h"
#include "GraphicsEngine/GraphicsMain/GraphicsMain.h"
#include "../Message/Console.h"
#include <unordered_map>
#include "CCellContents.h"

#define gravityVal 9.80665f 

namespace physics {
	CPhysicsEngine::CPhysicsEngine() :
		m_ColliderTree(std::make_unique<CColliderTree>())
	{

	}

	CPhysicsEngine::~CPhysicsEngine() {
		Release();
	}

	void CPhysicsEngine::Release() {
		ReleaseLoopResource();
		m_DynamicColliderList.clear();
		m_ColliderTree.release();
	}

	void CPhysicsEngine::ReleaseLoopResource() {
		m_CollisionInfoList.clear();
	}

	bool CPhysicsEngine::Initialize() {
		if (!m_ColliderTree->MakeColliderTree())return false;

		for (const auto& Collider : m_ColliderTree->GetColliderList()) {
			if (Collider->GetObjectType() == EObjectType::DYNAMIC)m_DynamicColliderList.push_back(Collider);
		}

		return true;
	}

	bool CPhysicsEngine::UpdatePhysics() {
		// 各コライダーごとの処理
		for (auto& CellContents : m_ColliderTree->GetCellContentsList()) {
			if (CellContents->GetCollider()->GetObjectType() != EObjectType::DYNAMIC)continue;

			if (!AddForce(CellContents->GetCollider()))return false; // 力を加える
			if (!UpdatePosition(CellContents->GetCollider()))return false; // 位置の更新
			
			// 更新された位置をもとにコライダーツリーを更新する
			if (!CellContents->RemoveFromCell())return false;
			if (!m_ColliderTree->RegistCellContents2Cell(CellContents))return false;
			
			ReleaseLoopResource(); // このループで作成したコリジョンのリソースを開放する
		}

		// コライダーツリーから衝突リストを取得する
		std::unordered_map<ACollider*, ACollider*> CollisionMap;
		if (!m_ColliderTree->GetCollisionAllMap(CollisionMap))return false;

		// 衝突リストごとに衝突判定を行う
		if (!DetectCollider(CollisionMap))return false;

		return true;
	}

	void CPhysicsEngine::AddCollider(EColliderType ColliderType, EObjectType ObjectType, GameObject* Obj) {
		m_ColliderTree->AddCollider(ColliderType, ObjectType, Obj);
	}

	bool CPhysicsEngine::AddForce(std::shared_ptr<ACollider> Collider) {
		glm::vec3 force = glm::vec3(0.0f);
		glm::vec3 velocity = Collider->GetVelocity();

		// 加わる力を計算する
		force += Collider->GetMass() * (-1.0f * glm::vec3(0.0f, gravityVal,0.0f));
		Collider->SetForce(force);

		// 速度
		velocity += (Collider->GetForce() / Collider->GetMass()) * GraphicsMain::GetInstance()->deltaTime;
		Collider->SetVelocity(velocity);

		return true;
	}

	//
	bool CPhysicsEngine::DetectCollider(const std::unordered_map<ACollider*, ACollider*>& CollisionMap) {
		for (auto& Collision : CollisionMap) {
			
			auto CollsionInfo = Collision.second->DetectCollider(Collision.first);
			std::unordered_map<ACollider*, std::shared_ptr<SCollisionInfo>> CollisionInfoList;
			
			if (CollsionInfo) {
				// 
				CollisionInfoList.emplace(Collision.second,CollsionInfo);

				// 貫通の解消
				if (!ResolvePenetration(Collision.first, CollisionInfoList))return false;

				// 位置の更新
				if (!UpdatePosition(Collision.first))return false;
			}
		}
		return true;
	}

	bool CPhysicsEngine::ResolvePenetration(ACollider* Collider, std::unordered_map<ACollider*, std::shared_ptr<SCollisionInfo>> CollisionInfoList) {

		for (const auto& Info : CollisionInfoList) {
			if (Info.second->m_IsCollided) {
				//glm::vec3 ColliderVel = Collider->GetVelocity();
				// 	Collider->SetVelocity(ColliderVel+glm::vec3(0.0f,5.0f,0.0f));

				// 各コライダーの速度
				glm::vec3 va = Collider->GetVelocity();
				glm::vec3 vb = Info.first->GetVelocity();

				// 各コライダーの質量
				float ma = Collider->GetMass();
				float mb = Info.first->GetMass();

				// 相対速度
				glm::vec3 vab = vb - va;

				// ばね定数
				float k = 1.0f;

				// 反発係数
				//float e = -1.0f * ((0.0f - k * Info.second->m_depth) / (glm::dot(vab, Info.second->m_normal)));
				//e = glm::max(0.0f, glm::min(1.0f/GraphicsMain::GetInstance()->deltaTime, e));
				float e = 1.0f;

				// 撃力係数の算出
				float c = (ma * mb / (ma + mb)) * ((1.0f + e) * glm::dot(vab, Info.second->m_normal) - k * Info.second->m_depth);

				// 撃力を加えて貫通を解決する
					// 衝突する側
				va = (c * Info.second->m_normal / ma);
				Collider->SetVelocity(va);

				// 衝突される側
				if (Info.first->GetObjectType() == EObjectType::DYNAMIC) {
					vb = (c * Info.second->m_normal / mb);
					Info.first->SetVelocity(vb);
				}
			}
		}

		return true;
	}

	//
	bool CPhysicsEngine::UpdatePosition(std::shared_ptr<ACollider> Collider) {
		glm::vec3 UpdatePos = Collider->GetTransform()->GetPosition();
		UpdatePos += Collider->GetVelocity()* GraphicsMain::GetInstance()->deltaTime;

		Collider->GetTransform()->SetPosition(UpdatePos);
		return true;
	}
	
	//
	bool CPhysicsEngine::UpdatePosition(ACollider* Collider) {
		glm::vec3 UpdatePos = Collider->GetTransform()->GetPosition();
		UpdatePos += Collider->GetVelocity()* GraphicsMain::GetInstance()->deltaTime;

		Collider->GetTransform()->SetPosition(UpdatePos);
		return true;
	}
}