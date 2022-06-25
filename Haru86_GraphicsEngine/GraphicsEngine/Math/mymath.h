#pragma once
#include <math.h>

namespace mymath {
	// float //////////////////////////////////////////////
	static float abs(float val) {
		return (val < 0.0f) ? val * (-1.0f) : val;
	}
	
	// vec3 ////////////////////////////////////////////////
	struct vec3
	{
		float x;
		float y;
		float z;

		float vectorVal[3];

		vec3(float xv, float yv, float zv) :
			x(xv),
			y(yv),
			z(zv),
			vectorVal{ xv,yv,zv }
		{
		}

		float& operator[](int index) {
			return vectorVal[index];
		}

		vec3 operator/(float v) {
			return vec3(x / v, y / v, z / v);
		}

		static vec3 normalize(vec3& v) {
			float length = sqrtf(powf(v.x, 2.0f) + powf(v.y, 2.0f) + powf(v.z, 2.0f));
			return (length <= 0.0f) ? v : v / length;
		}
	};
	
	// vec2 ///////////////////////////////////////////////
	struct vec2
	{
		float x;
		float y;

		float vectorVal[2];

		vec2(float xv, float yv) :
			x(xv),
			y(yv),
			vectorVal{ xv,yv }
		{
		}

		float& operator[](int index) {
			return vectorVal[index];
		}
	};

	// mat4 ///////////////////////////////////////////////
	struct mat4
	{
		//
		float matVal[16];

		//
		mat4() :
			matVal{
				1.0f,0.0f,0.0f,0.0f,
				0.0f,1.0f,0.0f,0.0f,
				0.0f,0.0f,1.0f,0.0f,
				0.0f,0.0f,0.0f,1.0f
		}
		{
		}

		float& operator[](int index) {
			return matVal[index];
		}

		mat4 operator*(mat4& m) {
			mat4 result = mat4();

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
		static mat4 translate(const vec3& trans) {
			mat4 result = mat4();

			result[12] = trans.x;
			result[13] = trans.y;
			result[14] = trans.z;

			return result;
		}

		static mat4 rotation(const vec3& rot) { // クォータニオンの考慮が大変なので後で実装する
			mat4 result = mat4();
			return result;
		}

		static mat4 scale(const vec3& s) {
			mat4 result = mat4();

			result[0] = s.x;
			result[5] = s.y;
			result[10] = s.z;

			return result;
		}

		static mat4 lookAt(vec3 pos, vec3 center, vec3 up) {
			vec2 test(0.0, 0.0);
			float a = test[0];
			
			mat4 result = mat4();

			float eyeX = pos[0], eyeY = pos[1], eyeZ = pos[2],
				upX = up[0], upY = up[1], upZ = up[2],
				centerX = center[0], centerY = center[1], centerZ = center[2];
			if (eyeX == centerX && eyeY == centerY && eyeZ == centerZ) { return result; }
			float x0, x1, x2, y0, y1, y2, z0, z1, z2, l;
			z0 = eyeX - center[0]; z1 = eyeY - center[1]; z2 = eyeZ - center[2];
			l = 1 / sqrtf(z0 * z0 + z1 * z1 + z2 * z2);
			z0 *= l; z1 *= l; z2 *= l;
			x0 = upY * z2 - upZ * z1;
			x1 = upZ * z0 - upX * z2;
			x2 = upX * z1 - upY * z0;
			l = sqrtf(x0 * x0 + x1 * x1 + x2 * x2);
			if (!l) {
				x0 = 0; x1 = 0; x2 = 0;
			}
			else {
				l = 1 / l;
				x0 *= l; x1 *= l; x2 *= l;
			}
			y0 = z1 * x2 - z2 * x1; y1 = z2 * x0 - z0 * x2; y2 = z0 * x1 - z1 * x0;
			l = sqrtf(y0 * y0 + y1 * y1 + y2 * y2);
			if (!l) {
				y0 = 0; y1 = 0; y2 = 0;
			}
			else {
				l = 1 / l;
				y0 *= l; y1 *= l; y2 *= l;
			}
			result[0] = x0; result[1] = y0; result[2] = z0; result[3] = 0;
			result[4] = x1; result[5] = y1; result[6] = z1; result[7] = 0;
			result[8] = x2; result[9] = y2; result[10] = z2; result[11] = 0;
			result[12] = -(x0 * eyeX + x1 * eyeY + x2 * eyeZ);
			result[13] = -(y0 * eyeX + y1 * eyeY + y2 * eyeZ);
			result[14] = -(z0 * eyeX + z1 * eyeY + z2 * eyeZ);
			result[15] = 1;

			return result;
		}

		static mat4 perspective(float fov, float ratio, float near, float far) {
			mat4 result = mat4();

			float PI = 3.14f;
			float t = near * tanf(fov * PI / 360.0f);
			float r = t * ratio;
			float a = r * 2, b = t * 2, c = far - near;
			result[0] = near * 2 / a;
			result[1] = 0;
			result[2] = 0;
			result[3] = 0;
			result[4] = 0;
			result[5] = near * 2 / b;
			result[6] = 0;
			result[7] = 0;
			result[8] = 0;
			result[9] = 0;
			result[10] = -(far + near) / c;
			result[11] = -1;
			result[12] = 0;
			result[13] = 0;
			result[14] = -(far * near * 2) / c;
			result[15] = 0;

			return result;
		}
	};

	struct quat
	{

	};
}

