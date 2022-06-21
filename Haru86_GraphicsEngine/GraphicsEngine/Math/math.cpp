#include "math.h"

namespace math {
	// float
	float abs(float val) {
		return (val < 0.0f) ? val * (-1.0f) : val;
	}



	// vec3
	vec3::vec3(float xv, float yv, float zv) :
		x(xv),
		y(yv),
		z(zv),
		vectorVal{xv,yv,zv}
	{
	}

	float& vec3::operator[](int index) {
		return vectorVal[index];
	}
	
	// vec2
	vec2::vec2(float xv, float yv) :
		x(xv),
		y(yv),
		vectorVal{xv,yv}
	{
	}

	float& vec2::operator[](int index) {
		return vectorVal[index];
	}

	// mat4
	mat4::mat4() :
		matVal{
			1.0f,0.0f,0.0f,0.0f,
			0.0f,1.0f,0.0f,0.0f,
			0.0f,0.0f,1.0f,0.0f,
			0.0f,0.0f,0.0f,1.0f
		}
	{
	}

	float& mat4::operator[](int index) {
		return matVal[index];
	}

	mat4 mat4::operator*(mat4& m) {
		mat4 result=mat4();
		
		int r0 = 0;
		int r1 = 4;
		int r2 = 8;
		int r3 = 12;

		//
		float a0 = matVal[r0 + 0] * m[r0 + 0] + matVal[r1 + 0] * m[r0 + 1] + matVal[r2 + 0] * m[r0 + 2] + matVal[r3 + 0] * m[r0 + 3];
		float a1 = matVal[r0 + 1] * m[r0 + 0] + matVal[r1 + 1] * m[r0 + 1] + matVal[r2 + 1] * m[r0 + 2] + matVal[r3 + 1] * m[r0 + 3];
		float a2 = matVal[r0 + 2] * m[r0 + 0] + matVal[r1 + 2] * m[r0 + 1] + matVal[r2 + 2] * m[r0 + 2] + matVal[r3 + 2] * m[r0 + 3];
		float a3 = matVal[r0 + 3] * m[r0 + 0] + matVal[r1 + 3] * m[r0 + 1] + matVal[r2 + 3] * m[r0 + 2] + matVal[r3 + 3] * m[r0 + 3];

		float a4 = matVal[r0 + 0] * m[r1 + 0] + matVal[r1 + 0] * m[r1 + 1] + matVal[r2 + 0] * m[r1 + 2] + matVal[r3 + 0] * m[r1 + 3];
		float a5 = matVal[r0 + 1] * m[r1 + 0] + matVal[r1 + 1] * m[r1 + 1] + matVal[r2 + 1] * m[r1 + 2] + matVal[r3 + 1] * m[r1 + 3];
		float a6 = matVal[r0 + 2] * m[r1 + 0] + matVal[r1 + 2] * m[r1 + 1] + matVal[r2 + 2] * m[r1 + 2] + matVal[r3 + 2] * m[r1 + 3];
		float a7 = matVal[r0 + 3] * m[r1 + 0] + matVal[r1 + 3] * m[r1 + 1] + matVal[r2 + 3] * m[r1 + 2] + matVal[r3 + 3] * m[r1 + 3];

		float a8 = matVal[r0 + 0] * m[r2 + 0] + matVal[r1 + 0] * m[r2 + 1] + matVal[r2 + 0] * m[r2 + 2] + matVal[r3 + 0] * m[r2 + 3];
		float a9 = matVal[r0 + 1] * m[r2 + 0] + matVal[r1 + 1] * m[r2 + 1] + matVal[r2 + 1] * m[r2 + 2] + matVal[r3 + 1] * m[r2 + 3];
		float a10 = matVal[r0 + 2] * m[r2 + 0] + matVal[r1 + 2] * m[r2 + 1] + matVal[r2 + 2] * m[r2 + 2] + matVal[r3 + 2] * m[r2 + 3];
		float a11 = matVal[r0 + 3] * m[r2 + 0] + matVal[r1 + 3] * m[r2 + 1] + matVal[r2 + 3] * m[r2 + 2] + matVal[r3 + 3] * m[r2 + 3];

		float a12 = matVal[r0 + 0] * m[r3 + 0] + matVal[r1 + 0] * m[r3 + 1] + matVal[r2 + 0] * m[r3 + 2] + matVal[r3 + 0] * m[r3 + 3];
		float a13 = matVal[r0 + 1] * m[r3 + 0] + matVal[r1 + 1] * m[r3 + 1] + matVal[r2 + 1] * m[r3 + 2] + matVal[r3 + 1] * m[r3 + 3];
		float a14 = matVal[r0 + 2] * m[r3 + 0] + matVal[r1 + 2] * m[r3 + 1] + matVal[r2 + 2] * m[r3 + 2] + matVal[r3 + 2] * m[r3 + 3];
		float a15 = matVal[r0 + 3] * m[r3 + 0] + matVal[r1 + 3] * m[r3 + 1] + matVal[r2 + 3] * m[r3 + 2] + matVal[r3 + 3] * m[r3 + 3];


		//
		result[0] = a0;
		result[1] = a1;
		result[2] = a2;
		result[3] = a3;
		result[4] = a4;
		result[5] = a5;
		result[6] = a6;
		result[7] = a7;
		result[8] = a8;
		result[9] = a9;
		result[10] = a10;
		result[11] = a11;
		result[12] = a12;
		result[13] = a13;
		result[14] = a14;
		result[15] = a15;

		return result;
	}


	// static
	mat4 mat4::translate(const vec3& trans) {
		mat4 result = mat4();
		
		result[12] = trans.x;
		result[13] = trans.y;
		result[14] = trans.z;

		return result;
	}

	mat4 mat4::rotation(const vec3& rot) { // クォータニオンの考慮が大変なので後で実装する
		mat4 result = mat4();
		return result;
	}

	mat4 mat4::scale(const vec3& s) {
		mat4 result = mat4();

		result[0] = s.x;
		result[5] = s.y;
		result[10] = s.z;

		return result;
	}

	mat4 mat4::lookAt(const vec3& pos, const vec3& center, const vec3& up) {
		mat4 result = mat4();
		return result;
	}

	mat4 mat4::perspective(float fov, float ratio, float near, float far) {
		mat4 result = mat4();
		return result;
	}

}