#pragma once
#include "CTrack.h"
#include <memory>

class TransformComponent;

namespace animation {
	class TransformTrack
	{
	protected:
		unsigned int m_Id;
		std::shared_ptr<VectorTrack> m_Position;
		std::shared_ptr<QuaternionTrack> m_Rotation;
		std::shared_ptr<VectorTrack> m_Scale;
	public:
		TransformTrack();
		~TransformTrack() = default;

		unsigned int GetID()const;
		void SetId(unsigned int id);

		std::shared_ptr<VectorTrack>& GetPositionTrack();
		std::shared_ptr<QuaternionTrack>& GetRotationTrack();
		std::shared_ptr<VectorTrack>& GetScaleTrack();

		float GetStartTime();
		float GetEndTime();

		bool IsValid();

		std::shared_ptr<TransformComponent> Sample(const std::shared_ptr<TransformComponent>& ref, float time, bool looping);
	};
}