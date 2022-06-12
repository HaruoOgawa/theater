#include "CBoxCollider.h"
#include "GraphicsEngine/Component/TransformComponent.h"

namespace physics {
	CBoxCollider::CBoxCollider(EObjectType ObjectType, TransformComponent* Transform,
		glm::vec3 CenterPos, std::vector<glm::vec3> LocalAxis, glm::vec3 LocalAxisSize) 
		: ACollider(ObjectType,EColliderType::Box, Transform),
		m_CenterPos(CenterPos),
		m_LocalAxis(LocalAxis),
		m_LocalAxisSize(LocalAxisSize)
	{
		// コライダーツリーのためのBoundsの生成
		float maxSize = glm::max(m_LocalAxisSize.x, glm::max(m_LocalAxisSize.y, m_LocalAxisSize.z))*2.0f;
		glm::vec3 MinPos = m_CenterPos + maxSize * glm::normalize(glm::vec3(1.0f, -1.0f, 1.0f));
		glm::vec3 MaxPos = m_CenterPos + maxSize * glm::normalize(glm::vec3(-1.0f, 1.0f, -1.0f));
		m_BB4Tree = std::make_shared<Bounds>(m_CenterPos, MinPos, MaxPos);
	}

	CBoxCollider::~CBoxCollider() {

	}

	std::shared_ptr<SCollisionInfo> CBoxCollider::DetectBoxCollider(ACollider* Collider) {
		std::shared_ptr<SCollisionInfo> CollisionInfo = nullptr;

		return CollisionInfo;
	}

	std::shared_ptr<SCollisionInfo> CBoxCollider::DetectSphereCollider(ACollider* Collider) {
		std::shared_ptr<SCollisionInfo> CollisionInfo = nullptr;

		// 衝突する側の座標を衝突される側の座標系での座標に変換する
		glm::vec3 ColliderPos =  Collider->GetTransform()->GetPosition();
		ColliderPos = (m_Transform->GetInvMMatrix() * glm::vec4(ColliderPos.x, ColliderPos.y, ColliderPos.z, 1.0f));

		// 最近接点を計算する
		glm::vec3 closestPoint;
		GetClosestPointAtOBB(ColliderPos, closestPoint);

		// 衝突情報をまとめる
		glm::vec3 CollidedNormal = closestPoint-ColliderPos ;
		float CollidedDepth;
		//float CollidedDepth = glm::dot(CollidedNormal, CollidedNormal);

		bool IsCollided = Collider->CalcIsingCollided(CollidedNormal,CollidedDepth);
		if (IsCollided) {
			// ワールド座標系に戻す
			CollidedNormal = (glm::mat4_cast(m_Transform->GetRotation())*glm::vec4(CollidedNormal.x, CollidedNormal.y, CollidedNormal.z,1.0f));
			//
			CollidedDepth = glm::sqrt(CollidedDepth);
			//
			CollisionInfo = std::make_shared<SCollisionInfo>(IsCollided, CollidedDepth, CollidedNormal);
		}

		return CollisionInfo;
	}

	bool CBoxCollider::CalcIsingCollided(glm::vec3& CollidedNormal, float& CollidedDepth) {
		return false;
	}

	void CBoxCollider::GetClosestPointAtOBB(const glm::vec3& pt, glm::vec3& closestPoint) {

		//
		glm::vec3 dir = pt - m_CenterPos;

		// 中心からスタート
		closestPoint = m_CenterPos;

		for (int i = 0; i < 3;i++) {
			// OBBのローカル軸への射影ベクトルの長さ
			float dist = glm::dot(dir, m_LocalAxis[i]);

			// ボックスを超えている場合はクランプする
			if (dist > m_LocalAxisSize[i])dist = m_LocalAxisSize[i];
			if (dist < -m_LocalAxisSize[i])dist = -m_LocalAxisSize[i];

			// ベクトルを進める
			closestPoint += dist * m_LocalAxis[i];
		}
	}

}