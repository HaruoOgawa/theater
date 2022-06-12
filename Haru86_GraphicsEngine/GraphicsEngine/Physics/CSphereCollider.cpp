#include "CSphereCollider.h"
#include "GraphicsEngine/Component/TransformComponent.h"

namespace physics {
	CSphereCollider::CSphereCollider(EObjectType ObjectType, TransformComponent* Transform, glm::vec3 CenterPos, float Radius)
		: ACollider(ObjectType,EColliderType::Sphere, Transform),
		m_CenterPos(CenterPos),
		m_Radius(Radius)
	{
		// コライダーツリーのためのBoundsの生成
		glm::vec3 MinPos = m_CenterPos + m_Radius*glm::normalize(glm::vec3(1.0f, -1.0f, 1.0f));
		glm::vec3 MaxPos = m_CenterPos + m_Radius*glm::normalize(glm::vec3(-1.0f, 1.0f, -1.0f));
		m_BB4Tree = std::make_shared<Bounds>(m_CenterPos, MinPos, MaxPos);
	}
	
	CSphereCollider::~CSphereCollider() {

	}

	std::shared_ptr<SCollisionInfo> CSphereCollider::DetectBoxCollider(ACollider* Collider) {
		std::shared_ptr<SCollisionInfo> CollisionInfo = nullptr;

		return CollisionInfo;
	}
	
	std::shared_ptr<SCollisionInfo> CSphereCollider::DetectSphereCollider(ACollider* Collider) {
		std::shared_ptr<SCollisionInfo> CollisionInfo = nullptr;

		// 間違ってはいるが、ひとまずこれで計算する

		// 衝突する側の座標を衝突される側の座標系での座標に変換する
		glm::vec3 ColliderPos = Collider->GetTransform()->GetPosition();
		ColliderPos = (m_Transform->GetInvMMatrix() * glm::vec4(ColliderPos.x, ColliderPos.y, ColliderPos.z, 1.0f));

		// 最近接点を計算する
		glm::vec3 closestPoint;
		//GetClosestPointAtOBB(ColliderPos, closestPoint);

		// 衝突情報をまとめる
		glm::vec3 CollidedNormal = m_CenterPos - ColliderPos;
		float CollidedDepth;
		//float CollidedDepth = glm::dot(CollidedNormal, CollidedNormal);

		bool IsCollided = Collider->CalcIsingCollided(CollidedNormal, CollidedDepth);
		if (IsCollided) {
			// ワールド座標系に戻す
			CollidedNormal = (glm::mat4_cast(m_Transform->GetRotation()) * glm::vec4(CollidedNormal.x, CollidedNormal.y, CollidedNormal.z, 1.0f));
			//
			CollidedDepth = glm::sqrt(CollidedDepth);
			//
			CollisionInfo = std::make_shared<SCollisionInfo>(IsCollided, CollidedDepth, CollidedNormal);
		}

		return CollisionInfo;
	}

	bool CSphereCollider::CalcIsingCollided(glm::vec3& CollidedNormal, float& CollidedDepth) {
		CollidedNormal = CollidedNormal + (-1.0f)*(glm::normalize(CollidedNormal)) * m_Radius;
		CollidedDepth = glm::dot(CollidedNormal, CollidedNormal);

		return (CollidedDepth < m_Radius* m_Radius);
	}
}