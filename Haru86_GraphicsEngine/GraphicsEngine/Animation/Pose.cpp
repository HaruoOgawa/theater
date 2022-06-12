#include "Pose.h"
#include "GraphicsEngine/Component/TransformComponent.h"
namespace animation {
	Pose::Pose() {
	}

	Pose::Pose(const Pose& p) {
		*this = p;
	}

	Pose::Pose(unsigned int numJoints) {
		Resize(numJoints);
	}

	Pose& Pose::operator=(const Pose& p) {
		if (&p==this) {
			return *this;
		}

		if (m_Parents.size() != p.m_Parents.size()) {
			m_Parents.resize(p.m_Parents.size(),-1);
		}

		if (m_Joints.size() != p.m_Joints.size()) {
			m_Joints.resize(p.m_Joints.size());
		}

		if (m_Parents.size() != 0) {
			std::memcpy(&m_Parents[0],&p.m_Parents[0],sizeof(int)*m_Parents.size());
		}

		if (m_Joints.size()!=0) {
			std::memcpy(&m_Joints[0],&p.m_Joints[0],sizeof(TransformComponent) * m_Joints.size());
		}

		return *this;
	}

	void Pose::Resize(unsigned int size) {
		m_Parents.resize(size, -1);
		m_Joints.resize(size);
	}

	unsigned int Pose::Size()const {
		return m_Joints.size();
	}

	int Pose::GetParent(unsigned int index)const {
		return m_Parents[index];
	}

	void Pose::SetParent(unsigned int index, int parent) {
		m_Parents[index] = parent;
	}

	const std::shared_ptr<TransformComponent> Pose::GetLocalTransform(unsigned int index)const {
		return m_Joints[index];
	}

	void Pose::SetLocalTransform(unsigned int index, const std::shared_ptr<TransformComponent>& transform) {
		m_Joints[index] = transform;
	}

	// ルートの親要素から処理対象のノード(子要素)までのモデル行列の積を求める
	std::shared_ptr<TransformComponent> Pose::GetGloabalTransform(unsigned int index)const {
		std::shared_ptr<TransformComponent> result = m_Joints[index];
		for (int p = m_Parents[index]; p >= 0;p=m_Parents[p]) {
			result = TransformComponent::Combine(m_Joints[p],result);
		}

		return result;
	}

	const std::shared_ptr<TransformComponent>& Pose::operator[](unsigned int index)const {
		return GetGloabalTransform(index);
	}

	void Pose::GetMatrixPalette(std::vector<glm::mat4>& out) {
		unsigned int size = Size();
		if (out.size()!=size) {
			out.resize(size);
		}

		for (unsigned int i= 0; i < size; ++i) {
			const auto& t = GetGloabalTransform(i);
			// transformToMat4 => モデル行列の取得
			out[i] = t->GetMMatrix();
		}
	}

	bool Pose::operator==(const Pose& other) {
		if (m_Joints.size()!=other.m_Joints.size()) {
			return false;
		}

		if (m_Parents.size() != other.m_Parents.size()) {
			return false;
		}

		unsigned int size = (unsigned int)m_Joints.size();
		for (unsigned int i = 0; i < size;++i) {
			const auto& thisLocal = m_Joints[i];
			const auto& otherLocal = other.m_Joints[i];
			
			int thisParent = m_Parents[i];
			int otherParent = other.m_Parents[i];

			if (thisParent!=otherParent) {
				return false;
			}

			if (thisLocal->GetPosition()!=otherLocal->GetPosition()) {
				return false;
			}

			if (thisLocal->GetRotation() != otherLocal->GetRotation()) {
				return false;
			}

			if (thisLocal->GetScale() != otherLocal->GetScale()) {
				return false;
			}
		}

		return true;
	}

	bool Pose::operator!=(const Pose& other) {
		return !(*this == other);
	}
}