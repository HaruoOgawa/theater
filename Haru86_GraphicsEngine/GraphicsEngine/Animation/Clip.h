#pragma once
#include <vector>
#include <memory>
#include <string>

namespace animation {
	class TransformTrack;
	class Pose;

	class Clip
	{
	public:
		Clip();

		unsigned int GetIdAtIndex(unsigned int index)const;
		void SetIdAtIndex(unsigned int idx,unsigned int id);
		unsigned int Size()const;

		float Sample(std::shared_ptr<Pose>& outPose,float time);
		std::shared_ptr<TransformTrack>& operator[](unsigned int joint);
		std::shared_ptr<TransformTrack>& GetTrack(unsigned int joint);

		void RecalculateDuration();

		std::string GetName()const;
		void SetName(const std::string& inNewName);
		float GetDuration()const;
		float GetStartTime()const;
		float GetEndTime()const;
		bool GetLooping()const;
		void SetLooping(bool inLooping);
	protected:
		std::vector<std::shared_ptr<TransformTrack>> m_Tracks;
		std::string m_Name;
		float m_StartTime;
		float m_EndTime;
		bool m_Looping;

		//
		float AdjustTimeToFitRange(float inTime);
	};
}