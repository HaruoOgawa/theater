#pragma once

namespace math {
	static float abs(float val);

	struct vec3
	{
		float x;
		float y;
		float z;

		float vectorVal[3];

		vec3(float xv, float yv, float zv);
		float& operator[](int index);
		vec3 operator/(float v);

		static vec3 normalize(vec3& v);
	};
	
	struct vec2
	{
		float x;
		float y;

		float vectorVal[2];

		vec2(float xv, float yv);
		float& operator[](int index);
	};

	struct mat4
	{
		//
		float matVal[16];

		//
		mat4();
		float& operator[](int index);
		mat4 operator*(mat4& m);

		// static
		static mat4 translate(const vec3& trans);
		static mat4 rotation(const vec3& rot);
		static mat4 scale(const vec3& s);
		static mat4 lookAt(const vec3& pos, const vec3& center, const vec3& up);
		static mat4 perspective(float fov, float ratio, float near, float far);
	};

	struct quat
	{

	};
}

