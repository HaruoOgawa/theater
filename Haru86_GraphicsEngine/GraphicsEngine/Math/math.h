#pragma once

namespace math {
	class vec3
	{
		float val[3];
	public:
		// public func
		vec3(float xval, float yval, float zval);
		~vec3();

		// static func
		static vec3 abs(const vec3& v);

		// variable
		float x;
		float y;
		float z;
	};
}

