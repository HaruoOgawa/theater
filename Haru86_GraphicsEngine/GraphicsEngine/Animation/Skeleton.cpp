#include "Skeleton.h"
#include "GraphicsEngine/Component/TransformComponent.h"

namespace animation {
	Skeleton::Skeleton() 
	{
	}

	Skeleton::Skeleton(const std::shared_ptr<Pose>& rest, const std::shared_ptr<Pose>& bind, const std::vector<std::string>& names) {
		Set(rest, bind, names);
	}

	void Skeleton::Set(const std::shared_ptr<Pose>& rest, const std::shared_ptr<Pose>& bind, const std::vector<std::string>& names) {
		m_RestPose = rest;
		m_BindPose = bind;
		m_JointNames = names;
		UpdateInverseBindPose();
	}

	std::shared_ptr<Pose>& Skeleton::GetBindPose() {
		return m_BindPose;
	}

	std::shared_ptr<Pose>& Skeleton::GetRestPose() {
		return m_RestPose;
	}

	std::vector<glm::mat4>& Skeleton::GetInvBindPose() {
		return m_InvBindPose;
	}

	std::vector<std::string>& Skeleton::GetJointNames() {
		return m_JointNames;
	}

	std::string& Skeleton::GetJointName(unsigned int index) {
		return m_JointNames[index];
	}

	void Skeleton::UpdateInverseBindPose() {
		unsigned int size = m_BindPose->Size();
		m_InvBindPose.resize(size);

		for (unsigned int i = 0; i < size;i++) {
			std::shared_ptr<TransformComponent> world = m_BindPose->GetGloabalTransform(i);
			m_InvBindPose[i] = glm::inverse(world->GetMMatrix());
		}
	}
}