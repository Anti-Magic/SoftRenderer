#pragma once

#include <cmath>

namespace SoftRenderer
{
	class Mat4;
	class Vec3;

	class Quat {
	public:
		float x;
		float y;
		float z;
		float w;

		Quat(float _x = 0, float _y = 0, float _z = 0, float _w = 1);

		static Quat fromAxisAngle(const Vec3& axis, float rad);
		static Quat fromEular(const Vec3& v);
		static Quat fromMat4(const Mat4& m);
		static Quat slerp(const Quat& left, const Quat& right, float amount);

		friend Quat operator*(const Quat& left, const Quat& right);
	};
}
