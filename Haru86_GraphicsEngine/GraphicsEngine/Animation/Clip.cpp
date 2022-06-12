#include "Clip.h"
#include "TransformTrack.h"
#include "Pose.h"
#include "GraphicsEngine/Component/TransformComponent.h"
#include <glm/glm.hpp>

namespace animation {
	Clip::Clip() :
		m_Name("No name given"),
		m_StartTime(0.0f),
		m_EndTime(0.0f),
		m_Looping(true)
	{
	}

	unsigned int Clip::GetIdAtIndex(unsigned int index)const {
		return m_Tracks[index]->GetID();
	}

	void Clip::SetIdAtIndex(unsigned int idx, unsigned int id) {
		return m_Tracks[idx]->SetId(id);
	}

	unsigned int Clip::Size()const {
		return (unsigned int)m_Tracks.size();
	}

	float Clip::Sample(std::shared_ptr<Pose>& outPose, float time) {
		if (GetDuration()==0.0f) {
			return 0.0;
		}
		time = AdjustTimeToFitRange(time);

		unsigned int size = m_Tracks.size();
		for (unsigned int i = 0; i < size; i++) {
			unsigned int j = m_Tracks[i]->GetID();
			const auto& local = outPose->GetLocalTransform(j);
			const auto& animated = m_Tracks[i]->Sample(local, time, m_Looping);

			outPose->SetLocalTransform(j, animated);
		}

		return time;
	}
	std::shared_ptr<TransformTrack>& Clip::operator[](unsigned int joint) {
		for (int i = 0, s = m_Tracks.size(); i < s; ++i) {
			if (m_Tracks[i]->GetID()==joint) {
				return m_Tracks[i];
			}
		}

		m_Tracks.push_back(std::make_shared<TransformTrack>());
		m_Tracks[m_Tracks.size() - 1]->SetId(joint);
		return m_Tracks[m_Tracks.size() - 1];
	}

	std::shared_ptr<TransformTrack>& Clip::GetTrack(unsigned int joint) {
		for (int i = 0, s = m_Tracks.size(); i < s; ++i) {
			if (m_Tracks[i]->GetID() == joint) {
				return m_Tracks[i];
			}
		}

		m_Tracks.push_back(std::make_shared<TransformTrack>());
		m_Tracks[m_Tracks.size() - 1]->SetId(joint);
		return m_Tracks[m_Tracks.size() - 1];
	}

	void Clip::RecalculateDuration() {
		m_StartTime = 0.0f;
		m_EndTime = 0.0f;

		bool startSet = false;
		bool endSet = false;

		unsigned int trackSize = m_Tracks.size();
		for (unsigned int i = 0; i < trackSize;i++) {
			if (m_Tracks[i]->IsValid()) {
				float startTime = m_Tracks[i]->GetStartTime();
				float endTime = m_Tracks[i]->GetEndTime();

				if (startTime<m_StartTime || !startSet) {
					m_StartTime = startTime;
					startSet = true;
				}

				if (endTime>m_EndTime || !endSet) {
					m_EndTime = endTime;
					endSet = true;
				}
			}
		}
	}

	std::string Clip::GetName()const {
		return m_Name;
	}

	void Clip::SetName(const std::string& inNewName) {
		m_Name = inNewName;
	}

	float Clip::GetDuration()const {
		return m_EndTime - m_StartTime;
	}

	float Clip::GetStartTime()const {
		return m_StartTime;
	}

	float Clip::GetEndTime()const {
		return m_EndTime;
	}

	bool Clip::GetLooping()const {
		return m_Looping;
	}

	void Clip::SetLooping(bool inLooping) {
		m_Looping = inLooping;
	}

	//
	float Clip::AdjustTimeToFitRange(float inTime) {
		if (m_Looping) {
			float duration = m_EndTime - m_StartTime;
			if (duration <= 0.0f) {
				return 0.0f;
			}

			inTime = glm::mod(inTime - m_StartTime, m_EndTime - m_StartTime);
			if (inTime<0.0f) {
				inTime += m_EndTime - m_StartTime;
			}
			inTime = inTime + m_StartTime;
		}
		else {
			if (inTime<m_StartTime) {
				inTime = m_StartTime;
			}

			if (inTime>m_EndTime) {
				inTime = m_EndTime;
			}
		}

		return inTime;
	}
}