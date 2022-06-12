#include "TransformTrack.h"

#include "GraphicsEngine/Component/TransformComponent.h"

namespace animation {
	TransformTrack::TransformTrack() :
		m_Id(0),
		m_Position(std::make_shared<VectorTrack>()),
		m_Rotation(std::make_shared<QuaternionTrack>()),
		m_Scale(std::make_shared<VectorTrack>())
	{
	}

	unsigned int TransformTrack::GetID()const {
		return m_Id;
	}
	void TransformTrack::SetId(unsigned int id) {
		m_Id = id;
	}

	std::shared_ptr<VectorTrack>& TransformTrack::GetPositionTrack() {
		return m_Position;
	}

	std::shared_ptr<QuaternionTrack>& TransformTrack::GetRotationTrack() {
		return m_Rotation;
	}

	std::shared_ptr<VectorTrack>& TransformTrack::GetScaleTrack() {
		return m_Scale;
	}

	float TransformTrack::GetStartTime() {
		float result = 0.0f;
		bool isSet = false;

		//
		if (m_Position->GetSize() > 1) {
			result = m_Position->GetStartTime();
			isSet = true;
		}

		//
		if (m_Rotation->GetSize() > 1) {
			float rotationStart = m_Rotation->GetStartTime();
			if (rotationStart < result || !isSet) {
				result = rotationStart;
				isSet = true;
			}
		}

		//
		if (m_Scale->GetSize() > 1) {
			float scaleStart = m_Scale->GetStartTime();
			if (scaleStart < result || !isSet) {
				result = scaleStart;
				isSet = true;
			}
		}

		//
		return result;
	}

	float TransformTrack::GetEndTime() {
		float result = 0.0f;
		bool isSet = false;

		//
		if (m_Position->GetSize() > 1) {
			result = m_Position->GetEndTime();
			isSet = true;
		}

		//
		if (m_Rotation->GetSize() > 1) {
			float rotationEnd = m_Rotation->GetEndTime();
			if (rotationEnd > result || !isSet) {
				result = rotationEnd;
				isSet = true;
			}
		}

		//
		if (m_Scale->GetSize() > 1) {
			float scaleEnd = m_Scale->GetEndTime();
			if (scaleEnd > result || !isSet) {
				result = scaleEnd;
				isSet = true;
			}
		}

		//
		return result;
	}

	bool TransformTrack::IsValid() {
		return (
			m_Position->GetSize() > 1 ||
			m_Rotation->GetSize() > 1 ||
			m_Scale->GetSize() > 1
		);
	}

	std::shared_ptr<TransformComponent> TransformTrack::Sample(const std::shared_ptr<TransformComponent>& ref, float time, bool looping) {
		std::shared_ptr<TransformComponent> result = ref;

		//
		if (m_Position->GetSize() > 1)result->SetPosition(m_Position->Sample(time, looping));
		if (m_Rotation->GetSize() > 1)result->SetRotation(m_Rotation->Sample(time, looping));
		if (m_Scale->GetSize() > 1)	 result->SetScale(m_Scale->Sample(time, looping));

		return result;
	}
}