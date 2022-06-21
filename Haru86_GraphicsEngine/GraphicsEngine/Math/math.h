#pragma once

namespace math {
	struct vec3
	{
		float x;
		float y;
		float z;

		float vectorVal[3];

		vec3(float xv, float yv, float zv);
		float operator[](int index);
	};
	
	struct vec2
	{
		float x;
		float y;

		float vectorVal[2];

		vec2(float xv, float yv);
		float operator[](int index);
	};

	struct mat4
	{

	};

	struct quat
	{

	};
}

