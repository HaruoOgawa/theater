#include "CTrack.h"
#include "CFrame.h"
#include <glm/gtc/quaternion.hpp>

namespace TrackHelpers {
	inline float Interpolate(float a, float b, float t) {
		return a + (b - a) * t;
	}

	inline glm::vec3 Interpolate(const glm::vec3& a, const glm::vec3& b, float t) {
		return glm::mix(a, b, t);
	}

	inline glm::quat Interpolate(const glm::quat& a, const glm::quat& b, float t) {
		glm::quat result = glm::mix(a, b, t);
		if (glm::dot(a, b) < 0) {
			result = glm::mix(a, -b, t);
		}
		return glm::normalize(result);
	}

	inline float AdjustHermiteResult(float f) {
		return f;
	}

	inline glm::vec3 AdjustHermiteResult(const glm::vec3& v) {
		return v;
	}

	inline glm::quat AdjustHermiteResult(const glm::quat& q) {
		return glm::normalize(q);
	}

	inline void Neighborhood(const float& a, float& b) {}
	inline void Neighborhood(const glm::vec3& a, glm::vec3& b) {}

	inline void Neighborhood(const glm::quat& a, glm::quat& b) {
		if (glm::dot(a, b) < 0) {
			b = -b;
		}
	}
}

namespace animation {
	template CTrack<float, 1>;
	template CTrack<glm::vec3, 3>;
	template CTrack<glm::quat, 4>;

	template<typename T,int N>
	CTrack<T,N>::CTrack() :
		m_InterpolationType(EInterpolationType::Linear)
	{
	}

	template<typename T,int N>
	CTrack<T, N>::~CTrack() {
		m_Frames.clear();
	}

	template<typename T, int N>
	float CTrack<T, N>::GetStartTime()const {
		return m_Frames[0]->m_Time;
	}

	template<typename T, int N>
	float CTrack<T, N>::GetEndTime()const {
		return m_Frames[m_Frames.size() - 1]->m_Time;
	}

	//
	template<typename T,int N>
	T CTrack<T,N>::Sample(float time, bool looping) {
		if (m_InterpolationType==EInterpolationType::Constant) {
			return SampleConstant(time, looping);
		}
		else if (m_InterpolationType==EInterpolationType::Linear) {
			return SampleLinear(time, looping);
		}
		else if(m_InterpolationType==EInterpolationType::Cubic) {
			return SampleCubic(time, looping);
		}
	}

	template<typename T,int N>
	std::shared_ptr<animation::CFrame<N>>& CTrack<T, N>::operator[](unsigned int index) {
		return m_Frames[index];
	}

	template<typename T, int N>
	std::shared_ptr<animation::CFrame<N>>& CTrack<T, N>::GetFrame(unsigned int index){
		return m_Frames[index];
	}

	//
	template<typename T, int N>
	T CTrack<T, N>::Hermite(float t, const T& p1, const T& s1, const T& _p2, const T& s2) {
		float tt = t * t;
		float ttt = tt * t;

		T p2 = _p2;
		TrackHelpers::Neighborhood(p1, p2);

		float h1 = 2.0f * ttt - 3.0f * tt + 1.0f;
		float h2 = -2.0f * ttt + 3.0f * tt;
		float h3 = ttt - 2.0f * tt + t;
		float h4 = ttt - tt;

		T result = p1 * h1 + p2 * h2 + s1 * h3 + s2 * h4;
		return TrackHelpers::AdjustHermiteResult(result);
	}

	template<typename T,int N>
	void CTrack<T, N>::Resize(unsigned int size) {
		m_Frames.resize(size);
		for (int i = 0; i < size;i++) {
			m_Frames[i] = std::make_shared<CFrame<N>>();
		}
	}

	template<typename T,int N>
	unsigned int CTrack<T, N>::GetSize()const {
		return m_Frames.size();
	}

	template<typename T, int N>
	EInterpolationType CTrack<T, N>::GetInterpolationType()const {
		return m_InterpolationType;
	}
	
	template<typename T, int N>
	void CTrack<T, N>::SetInterpolationType(EInterpolationType InterpolationType) {
		m_InterpolationType = InterpolationType;
	}

	//
	template<typename T,int N>
	int CTrack<T,N>::FrameIndex(float time, bool looping) {
		// 
		unsigned int size = (unsigned int)m_Frames.size();
		if (size<=1) {
			return -1;
		}

		// ループアニメーション
		if (looping) {
			float startTime = m_Frames[0]->m_Time;
			float endTime = m_Frames[size-1]->m_Time;
			float duration = endTime - startTime;

			// durationの幅で繰り返す
			// 正しく繰り返すために一旦startTimeで調整する 5s～12s / t=3s =>> 0s～7s / t=-2s / duration 7s
			time = glm::modf(time - startTime, duration);

			// 負の値をmodの範囲内の数値に修正する
			// t=5s
			if (time < 0.0f) {
				time += duration;
			}

			// 0基準ではないもとの時間軸へ戻す(正しstartTimeとendTimeの間でいい感じに折り返している)
			// 0s～7s / t=5s =>> 5s～12s / t=10s
			time = time + startTime;
		}
		else { // 繰り返さない
			// スタート時間よりtimeが小さいならアニメーションはまだ始まってすらいないので最初のフレームを返す
			if (time <= m_Frames[0]->m_Time) {
				return 0; 
			}

			// 最後から1つ前の終了時間(最後はループなら繋がる)を超しているのならその最後の1つ前のフレームを返す
			// Sample関数は現在のフレームとその次のフレームを計算に使用するため、最後から一つ前のフレームを指定しなければならない
			if (time >= m_Frames[size-2]->m_Time) {
				return (int)size - 2;
			}
		}

		// 単純に一番近い時間のフレームを探索する 上記↑↑↑の繰り返さない処理では端のフレームしか返さない
		for (int i = size - 1; i > -1; --i) {
			if (time>=m_Frames[i]->m_Time) {
				return i;
			}
		}
	}

	// アニメーション時間のなかにしっかり収めるための関数
	template<typename T,int N>
	float CTrack<T,N>::AdjustTimeToFitTrack(float time, bool loop) {
		unsigned int size = (unsigned int)m_Frames.size();
		
		//
		if (size <= 1) {
			return 0.0f;
		}

		//
		float startTime = m_Frames[0]->m_Time;
		float endTime = m_Frames[size - 1]->m_Time;
		float duration = endTime - startTime;
		if (duration <=0.0f) {
			return 0.0f;
		}

		//
		if (loop) {
			time = glm::modf(time - startTime, duration);

			//
			if (time < 0.0f) {
				time += duration;
			}
			time = time + startTime;
		}
		else {
			if (time<=m_Frames[0]->m_Time) {
				time = startTime;
			}

			if (time>=m_Frames[size-1]->m_Time) {
				time = endTime;
			}
		}

		return time;
	}

	template<>
	float CTrack<float, 1>::Cast(float* value) {
		return value[0];
	}

	template<>
	glm::vec3 CTrack<glm::vec3,3>::Cast(float* value) {
		return glm::vec3(value[0],value[1],value[2]);
	}

	template<>
	glm::quat CTrack<glm::quat,4>::Cast(float* value) {
		glm::quat r = glm::quat(value[0], value[1], value[2], value[3]);
		return glm::normalize(r);
	}

	// sample func
	template<typename T,int N>
	T CTrack<T, N>::SampleConstant(float time, bool looping) {
		int frame = FrameIndex(time ,looping);
		if (frame < 0 || frame>=(int)m_Frames.size()) {
			return T();
		}

		return Cast(&m_Frames[frame]->m_Value[0]);
	}
	
	template<typename T, int N>
	T CTrack<T, N>::SampleLinear(float time, bool looping) {
		//
		int thisFrame = FrameIndex(time, looping);
		if (thisFrame < 0 || thisFrame>=m_Frames.size()-1) {
			return T();
		}

		int nextFrame = thisFrame + 1;

		//
		float trackTime = AdjustTimeToFitTrack(time, looping);
		float thisTime = m_Frames[thisFrame]->m_Time;
		float frameDelta = m_Frames[nextFrame]->m_Time - thisTime;

		if (frameDelta<=0.0f) {
			return T();
		}

		//
		float t = (trackTime - thisTime) / frameDelta;

		T start = Cast(&m_Frames[thisFrame]->m_Value[0]);
		T end = Cast(&m_Frames[nextFrame]->m_Value[0]);

		return TrackHelpers::Interpolate(start, end, t);
	}
	
	template<typename T, int N>
	T CTrack<T, N>::SampleCubic(float time, bool looping) {
		//
		int thisFrame = FrameIndex(time, looping);
		if (thisFrame < 0 || thisFrame >= m_Frames.size() -1) {
			return T();
		}
		
		int nextFrame = thisFrame + 1;

		//
		float trackTime = AdjustTimeToFitTrack(time, looping);
		float thisTime = m_Frames[thisFrame]->m_Time;
		float frameDelta = m_Frames[nextFrame]->m_Time - thisTime;

		if (frameDelta <= 0.0f) {
			return T();
		}

		//
		float t = (trackTime - thisTime) / frameDelta;
		size_t fitSize = sizeof(float);

		//
		T point1 = Cast(&m_Frames[thisFrame]->m_Value[0]);
		T slope1;
		memcpy(&slope1, m_Frames[thisFrame]->m_Out, N * fitSize);
		slope1 = slope1 * frameDelta;

		//
		T point2 = Cast(&m_Frames[nextFrame]->m_Value[0]);
		T slope2;
		memcpy(&slope2, m_Frames[nextFrame]->m_In, N * fitSize);
		slope2 = slope2 * frameDelta;

		//
		return Hermite(t, point1, slope1, point2, slope2);
	}
}