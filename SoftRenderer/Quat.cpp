#include "Quat.h"
#include "Vec.h"
#include "Mat.h"

namespace SoftRenderer
{
	Quat::Quat(float _x, float _y, float _z, float _w)
		: x(_x), y(_y), z(_z), w(_w)
	{
	}

	Quat Quat::fromAxisAngle(const Vec3& axis, float rad)
	{
		float halfRad = rad * 0.5f;
		float sin = std::sin(halfRad);
		return Quat(
			sin * axis.x,
			sin * axis.y,
			sin * axis.z,
			std::cos(halfRad)
		);
	}

	Quat Quat::fromEular(const Vec3& v)
	{
		float sx = std::sin(v.x * 0.5f);
		float cx = std::cos(v.x * 0.5f);
		float sy = std::sin(v.y * 0.5f);
		float cy = std::cos(v.y * 0.5f);
		float sz = std::sin(v.z * 0.5f);
		float cz = std::cos(v.z * 0.5f);
		return Quat(
			(cx * sy * cz) + (sx * cy * sz),
			(sx * cy * cz) - (cx * sy * sz),
			(cx * cy * sz) - (sx * sy * cz),
			(cx * cy * cz) + (sx * sy * sz));
	}

	Quat Quat::fromMat4(const Mat4& m)
	{
		float m11 = m.d[0][0], m12 = m.d[1][0], m13 = m.d[2][0];
		float m21 = m.d[0][1], m22 = m.d[1][1], m23 = m.d[2][1];
		float m31 = m.d[0][2], m32 = m.d[1][2], m33 = m.d[2][2];
		float trace = m11 + m22 + m33;
		float s = 0;
		Quat res;
		if (trace > 0) {
			s = 0.5f / std::sqrt(trace + 1.0f);
			res.w = 0.25f / s;
			res.x = (m32 - m23) * s;
			res.y = (m13 - m31) * s;
			res.z = (m21 - m12) * s;
		}
		else if (m11 > m22 && m11 > m33) {
			s = 2.0f * std::sqrt(1.0f + m11 - m22 - m33);
			res.w = (m32 - m23) / s;
			res.x = 0.25f * s;
			res.y = (m12 + m21) / s;
			res.z = (m13 + m31) / s;
		}
		else if (m22 > m33) {
			s = 2.0f * std::sqrt(1.0f + m22 - m11 - m33);
			res.w = (m13 - m31) / s;
			res.x = (m12 + m21) / s;
			res.y = 0.25f * s;
			res.z = (m23 + m32) / s;
		}
		else {
			s = 2.0f * std::sqrt(1.0f + m33 - m11 - m22);
			res.w = (m21 - m12) / s;
			res.x = (m13 + m31) / s;
			res.y = (m23 + m32) / s;
			res.z = 0.25f * s;
		}
		return res;
	}

	Quat Quat::slerp(const Quat& left, const Quat& right, float amount) {
		float num2 = 0;
		float num3 = 0;
		float num4 = (((left.x * right.x) + (left.y * right.y)) + (left.z * right.z)) + (left.w * right.w);
		bool flag = false;
		if (num4 < 0) {
			flag = true;
			num4 = -num4;
		}
		if (num4 > 0.999999f) {
			num3 = 1 - amount;
			num2 = flag ? -amount : amount;
		}
		else {
			float num5 = std::acos(num4);
			float num6 = (1.0f / std::sin(num5));
			num3 = (std::sin((1.0f - amount) * num5)) * num6;
			num2 = flag ? ((-std::sin(amount * num5)) * num6) : ((std::sin(amount * num5)) * num6);
		}
		return Quat(
			(num3 * left.x) + (num2 * right.x),
			(num3 * left.y) + (num2 * right.y),
			(num3 * left.z) + (num2 * right.z),
			(num3 * left.w) + (num2 * right.w));
	}

	Quat operator*(const Quat& left, const Quat& right)
	{
		float ax = left.x;
		float ay = left.y;
		float az = left.z;
		float aw = left.w;
		float bx = right.x;
		float by = right.y;
		float bz = right.z;
		float bw = right.w;
		return Quat(
			ax * bw + aw * bx + ay * bz - az * by,
			ay * bw + aw * by + az * bx - ax * bz,
			az * bw + aw * bz + ax * by - ay * bx,
			aw * bw - ax * bx - ay * by - az * bz
		);
	}
}
