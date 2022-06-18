#include "math.h"

namespace math {
	vec3::vec3(float xval, float yval, float zval) :
		x(xval),
		y(yval),
		z(zval)
	{
		val[0] = xval;
		val[1] = yval;
		val[2] = zval;
	}

	vec3::~vec3()
	{
	}

	vec3 vec3::abs(const vec3& v) {
		vec3 rv = v;
		if (rv.x < 0.0f)rv.x *= -1.0f;
		if (rv.y < 0.0f)rv.y *= -1.0f;
		if (rv.z < 0.0f)rv.z *= -1.0f;
		return rv;
	}
}