#include "math.h"

namespace math {
	vec3::vec3(float xv, float yv, float zv) :
		x(xv),
		y(yv),
		z(zv),
		vectorVal{xv,yv,zv}
	{
	}

	float vec3::operator[](int index) {
		return vectorVal[index];
	}
	
	vec2::vec2(float xv, float yv) :
		x(xv),
		y(yv),
		vectorVal{xv,yv}
	{
	}

	float vec2::operator[](int index) {
		return vectorVal[index];
	}
}