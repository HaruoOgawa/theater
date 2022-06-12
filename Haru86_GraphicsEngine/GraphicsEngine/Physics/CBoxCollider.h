#pragma once
#include "ACollider.h"

namespace physics {
	class CBoxCollider :
		public ACollider
	{
		glm::vec3			   m_CenterPos;
		std::vector<glm::vec3> m_LocalAxis;
		glm::vec3              m_LocalAxisSize;
		
	public:
		CBoxCollider(EObjectType ObjectType, TransformComponent* Transform,
			glm::vec3 CenterPos, std::vector<glm::vec3> LocalAxis, glm::vec3 LocalAxisSize);
		virtual ~CBoxCollider();

		virtual std::shared_ptr<SCollisionInfo> DetectBoxCollider(ACollider*Collider) override;
		virtual std::shared_ptr<SCollisionInfo> DetectSphereCollider(ACollider*Collider) override;
		virtual bool CalcIsingCollided(glm::vec3& CollidedNormal, float& CollidedDepth)override;

	private:
		void GetClosestPointAtOBB(const glm::vec3& pt,glm::vec3& closestPoint);
	};
}


