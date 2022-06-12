#pragma once
#include <vector>
#include <memory>
#include <glm/glm.hpp>
#include "EInterpolationType.h"
#include "CFrame.h"

namespace animation {
	template<typename T,int N>
	class CTrack
	{
	public:
		CTrack();
		virtual ~CTrack();

		void Resize(unsigned int size);
		unsigned int GetSize()const;

		EInterpolationType GetInterpolationType()const;
		void SetInterpolationType(EInterpolationType InterpolationType);

		float GetStartTime()const;
		float GetEndTime()const;

		//
		T Sample(float time, bool looping);
		std::shared_ptr<animation::CFrame<N>>& operator[](unsigned int index);
		std::shared_ptr<animation::CFrame<N>>& GetFrame(unsigned int index);
			
		//
		T Hermite(float t, const T& p1, const T& s1, const T& _p2, const T& s2);
	protected:
		// variable
		std::vector<std::shared_ptr<CFrame<N>>> m_Frames;
		EInterpolationType m_InterpolationType;

		// function
		T SampleConstant(float time, bool looping);
		T SampleLinear(float time, bool looping);
		T SampleCubic(float time, bool looping);

		int FrameIndex(float time, bool looping);
		float AdjustTimeToFitTrack(float time,bool loop);

		T Cast(float* value);
	};

	typedef CTrack<float, 1> ScalarTrack;
	typedef CTrack<glm::vec3, 3> VectorTrack;
	typedef CTrack<glm::quat, 4> QuaternionTrack;
}


