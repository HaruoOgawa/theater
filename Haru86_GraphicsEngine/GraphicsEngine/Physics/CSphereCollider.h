#pragma once
#include "ACollider.h"

namespace physics {
	class CSphereCollider :
		public ACollider
	{
		glm::vec3 m_CenterPos;
		float     m_Radius;

	public:
		CSphereCollider(EObjectType ObjectType, TransformComponent* Transform, glm::vec3 CenterPos, float Radius);
		virtual ~CSphereCollider();

		virtual std::shared_ptr<SCollisionInfo> DetectBoxCollider(ACollider* Collider) override;
		virtual std::shared_ptr<SCollisionInfo> DetectSphereCollider(ACollider* Collider) override;
		virtual bool CalcIsingCollided(glm::vec3& CollidedNormal, float& CollidedDepth)override;
	};
}