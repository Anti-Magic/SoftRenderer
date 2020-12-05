#pragma once

#include <cstdint>
#include <cmath>

namespace SoftRenderer
{
	class Vec3;
	class Quat;
	class Mat4;

	class Mat4
	{
	public:
		float d[4][4];

		Mat4(float a00 = 1, float a01 = 0, float a02 = 0, float a03 = 0,
			float a10 = 0, float a11 = 1, float a12 = 0, float a13 = 0,
			float a20 = 0, float a21 = 0, float a22 = 1, float a23 = 0,
			float a30 = 0, float a31 = 0, float a32 = 0, float a33 = 1);

		Mat4 transpose();
		Mat4 inverse();

		static Mat4 translate(Vec3 v);
		static Mat4 rotate(Quat q);
		static Mat4 scale(Vec3 v);
		static Mat4 lookAt(Vec3 eye, Vec3 center, Vec3 up);
		static Mat4 perspective(float fovy, float aspect, float zNear, float zFar);
		static Mat4 ortho(float width, float height, float zNear, float zFar);

		friend Mat4 operator*(Mat4 left, Mat4 right);
	};
}
