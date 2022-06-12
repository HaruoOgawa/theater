#pragma once

#include "Pose.h"
#include <memory>
#include <vector>
#include <string>

namespace animation {
	class Skeleton
	{
	public:
		Skeleton();
		Skeleton(const std::shared_ptr<Pose>& rest,const std::shared_ptr<Pose>& bind,const std::vector<std::string>& names);

		void Set(const std::shared_ptr<Pose>& rest,const std::shared_ptr<Pose>& bind,const std::vector<std::string>& names);

		std::shared_ptr<Pose>& GetBindPose();
		std::shared_ptr<Pose>& GetRestPose();
		std::vector<glm::mat4>& GetInvBindPose();
		std::vector<std::string>& GetJointNames();
		std::string& GetJointName(unsigned int index);
	protected:
		//
		std::shared_ptr<Pose> m_RestPose; // rest => 静止 => その瞬間でのアニメーションのポーズ(1コマ??)
		std::shared_ptr<Pose> m_BindPose; // bindPose => T-Pose のこと
		std::vector<glm::mat4> m_InvBindPose;
		std::vector<std::string> m_JointNames;

		//
		void UpdateInverseBindPose();
	};
}