#include "ACollider.h"

namespace physics {
	SCollisionInfo::SCollisionInfo(bool IsCollided,float depth, glm::vec3 normal) :
		m_IsCollided(IsCollided),
		m_depth(depth),
		m_normal(normal) {
	}

	Bounds::Bounds(glm::vec3 CenterPos, glm::vec3 MinPos, glm::vec3 MaxPos) :
		m_CenterPos(CenterPos),
		m_MinPos(MinPos),
		m_MaxPos(MaxPos)
	{
	}

	ACollider::ACollider(EObjectType ObjectType, EColliderType ColliderType, TransformComponent* Transform) :
		m_ObjectType(ObjectType),
		m_ColliderType(ColliderType),
		m_Transform(Transform),
		m_Force(glm::vec3(0.0f)),
		m_Velocity(glm::vec3(0.0f)),
		m_Mass(1.0f),
		m_BB4Tree(nullptr)
	{
	}

	ACollider::~ACollider() {

	}

	EObjectType ACollider::GetObjectType()const {
		return m_ObjectType;
	}

	EColliderType ACollider::GetColliderType()const {
		return m_ColliderType;
	}

	std::shared_ptr<SCollisionInfo> ACollider::DetectCollider(ACollider* Collider) {
		std::shared_ptr<SCollisionInfo> CollisionInfo = nullptr;
		
		switch (Collider->GetColliderType())
		{
		case EColliderType::Box:
			CollisionInfo=DetectBoxCollider(Collider);
			break;

		case EColliderType::Sphere:
			CollisionInfo=DetectSphereCollider(Collider);
			break;

		default:
			break;
		}

		return CollisionInfo;
	}
	
	bool ACollider::CalcIsingCollided(glm::vec3& CollidedNormal, float& CollidedDepth) {
		return false;
	}

	std::shared_ptr<SCollisionInfo> ACollider::DetectBoxCollider(ACollider* Collider) {
		return nullptr;
	}
	
	std::shared_ptr<SCollisionInfo> ACollider::DetectSphereCollider(ACollider* Collider) {
		return nullptr;
	}

	void ACollider::SetForce(glm::vec3 Force) {
		m_Force = Force;
	}

	const glm::vec3& ACollider::GetForce()const {
		return m_Force;
	}
	
	void ACollider::SetVelocity(glm::vec3 Velocity) {
		m_Velocity = Velocity;
	}

	const glm::vec3& ACollider::GetVelocity()const {
		return m_Velocity;
	}

	void ACollider::SetMass(float Mass) {
		m_Mass = Mass;
	}

	float ACollider::GetMass()const {
		return m_Mass;
	}

	TransformComponent* ACollider::GetTransform() {
		return m_Transform;
	}

	const std::shared_ptr<Bounds>& ACollider::GetBounds()const {
		return m_BB4Tree;
	}
}
