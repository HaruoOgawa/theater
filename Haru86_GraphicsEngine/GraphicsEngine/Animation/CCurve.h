#pragma once
#include <glm/glm.hpp>

namespace animation {
	template<typename T>
	class Bezier {
	public:
		T P1;
		T C1;
		T P2;
		T C2;
	};
	
	template<typename T>
	inline T Interpolate(Bezier<T>& curve,float t) {
		return (
			curve.P1*((1.0f-t)*(1.0f-t)*(1.0f-t))+
			curve.C1*(3.0f*((1.0f-t) * (1.0f - t))*t)+
			curve.C2*(3.0f * (1.0f - t)*(t*t))+
			curve.P2*(t*t*t)
		);
	}

	template<typename T>
	T Hermite(float t,const T& p1, const T& s1, const T& p2, const T& s2) {
		return (
			p1*((1.0f+2.0f*t)*((1.0f-t)*(1.0f-t)))+
			s1*(t*((1.0f - t)* (1.0f - t)))+
			p2*((t*t)*(3.0f-2.0f*t))+
			s2*((t*t)*(t-1.0f))
		);
	}
}

