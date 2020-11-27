#include "Vec.h"
#include "Mat.h"
#include <cmath>

namespace SoftRenderer
{
	Vec2::Vec2(float _x, float _y)
		: x(_x), y(_y)
	{
	}

	float Vec2::magnitude()
	{
		return std::sqrt(x * x + y * y);
	}

	Vec2 Vec2::normalize()
	{
		float inv = 1.0f / magnitude();
		return (*this) * inv;
	}

	Vec2 operator+(const Vec2& left, const Vec2& right)
	{
		return Vec2(left.x + right.x, left.y + right.y);
	}

	Vec2 operator-(const Vec2& left, const Vec2& right)
	{
		return Vec2(left.x - right.x, left.y - right.y);
	}

	Vec2 operator*(const Vec2& left, const Vec2& right)
	{
		return Vec2(left.x * right.x, left.y * right.y);
	}

	Vec2 operator/(const Vec2& left, const Vec2& right)
	{
		return Vec2(left.x / right.x, left.y / right.y);
	}

	Vec2 operator*(const Vec2& left, float right)
	{
		return Vec2(left.x * right, left.y * right);
	}

	Vec2 operator*(float left, const Vec2& right)
	{
		return Vec2(left * right.x, left * right.y);
	}

	Vec3::Vec3(float _x, float _y, float _z)
		: x(_x), y(_y), z(_z)
	{
	}

	Vec3::Vec3(const Vec2& v, float _z)
		: x(v.x), y(v.y), z(_z)
	{
	}

	float Vec3::magnitude()
	{
		return std::sqrt(x * x + y * y + z * z);
	}

	Vec3 Vec3::normalize()
	{
		float inv = 1.0f / magnitude();
		return (*this) * inv;
	}

	float Vec3::dot(const Vec3& v) const
	{
		return x * v.x + y * v.y + z * v.z;
	}

	Vec3 Vec3::cross(const Vec3& v) const
	{
		return Vec3(
			y * v.z - z * v.y,
			z * v.x - x * v.z,
			x * v.y - y * v.x);
	}

	Vec3 operator+(const Vec3& left, const Vec3& right)
	{
		return Vec3(left.x + right.x, left.y + right.y, left.z + right.z);
	}

	Vec3 operator-(const Vec3& left, const Vec3& right)
	{
		return Vec3(left.x - right.x, left.y - right.y, left.z - right.z);
	}

	Vec3 operator*(const Vec3& left, const Vec3& right)
	{
		return Vec3(left.x * right.x, left.y * right.y, left.z * right.z);
	}

	Vec3 operator/(const Vec3& left, const Vec3& right)
	{
		return Vec3(left.x / right.x, left.y / right.y, left.z / right.z);
	}

	Vec3 operator*(const Vec3& left, float right)
	{
		return Vec3(left.x * right, left.y * right, left.z * right);
	}

	Vec3 operator*(float left, const Vec3& right)
	{
		return Vec3(left * right.x, left * right.y, left * right.z);
	}

	Vec4::Vec4(float _x, float _y, float _z, float _w)
		: x(_x), y(_y), z(_z), w(_w)
	{
	}

	Vec4::Vec4(const Vec2& v, float _z, float _w)
		: x(v.x), y(v.y), z(_z), w(_w)
	{
	}

	Vec4::Vec4(const Vec3& v, float _w)
		: x(v.x), y(v.y), z(v.z), w(_w)
	{
	}

	float Vec4::magnitude()
	{
		return std::sqrt(x * x + y * y + z * z + w * w);
	}

	Vec4 Vec4::normalize()
	{
		float inv = 1.0f / magnitude();
		return (*this) * inv;
	}

	Vec4 operator+(const Vec4& left, const Vec4& right)
	{
		return Vec4(left.x + right.x, left.y + right.y, left.z + right.z, left.w + right.w);
	}

	Vec4 operator-(const Vec4& left, const Vec4& right)
	{
		return Vec4(left.x - right.x, left.y - right.y, left.z - right.z, left.w - right.w);
	}

	Vec4 operator*(const Vec4& left, const Vec4& right)
	{
		return Vec4(left.x * right.x, left.y * right.y, left.z * right.z, left.w * right.w);
	}

	Vec4 operator/(const Vec4& left, const Vec4& right)
	{
		return Vec4(left.x / right.x, left.y / right.y, left.z / right.z, left.w / right.w);
	}

	Vec4 operator*(const Vec4& left, float right)
	{
		return Vec4(left.x * right, left.y * right, left.z * right, left.w * right);
	}

	Vec4 operator*(float left, const Vec4& right)
	{
		return Vec4(left * right.x, left * right.y, left * right.z, left * right.w);
	}

	Vec4 operator*(const Vec4& left, const Mat4& right)
	{
		return Vec4(
			left.x * right.d[0][0] + left.y * right.d[1][0] + left.z * right.d[2][0] + left.w * right.d[3][0], 
			left.x * right.d[0][1] + left.y * right.d[1][1] + left.z * right.d[2][1] + left.w * right.d[3][1], 
			left.x * right.d[0][2] + left.y * right.d[1][2] + left.z * right.d[2][2] + left.w * right.d[3][2], 
			left.x * right.d[0][3] + left.y * right.d[1][3] + left.z * right.d[2][3] + left.w * right.d[3][3]);
	}
}
