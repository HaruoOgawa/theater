#pragma once
#include <vector>
#include <memory>
#include "EColliderType.h"
#include "EObjectType.h"
#include <unordered_map>

class GameObject;

namespace physics {
	class CColliderTree;
	class ACollider;
	struct SCollisionInfo;

	class CPhysicsEngine
	{
		std::unique_ptr<CColliderTree> m_ColliderTree;
		std::vector<std::shared_ptr<ACollider>> m_DynamicColliderList;
		std::unordered_map<std::shared_ptr<ACollider>, std::shared_ptr<SCollisionInfo>> m_CollisionInfoList;

	public:
		CPhysicsEngine();
		~CPhysicsEngine();
		void Release();
		void ReleaseLoopResource();
		bool Initialize();
		bool UpdatePhysics();
		void AddCollider(EColliderType ColliderType,EObjectType ObjectType,GameObject* Obj);
	private:
		bool AddForce(std::shared_ptr<ACollider> Collider);
		bool DetectCollider(const std::unordered_map<ACollider*, ACollider*>& CollisionMap);
		bool ResolvePenetration(ACollider* Collider, std::unordered_map<ACollider*, std::shared_ptr<SCollisionInfo>> CollisionInfoList);
		bool UpdatePosition(std::shared_ptr<ACollider> Collider);
		bool UpdatePosition(ACollider* Collider);
	};
}

