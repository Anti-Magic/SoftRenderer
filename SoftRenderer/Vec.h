#pragma once

#include <cstdint>

namespace SoftRenderer
{
	class Mat4;

	class Vec2
	{
	public:
		float x;
		float y;

		Vec2(float _x = 0, float _y = 0);

		float magnitude();
		Vec2 normalize();

		friend Vec2 operator+(const Vec2& left, const Vec2& right);
		friend Vec2 operator-(const Vec2& left, const Vec2& right);
		friend Vec2 operator*(const Vec2& left, const Vec2& right);
		friend Vec2 operator/(const Vec2& left, const Vec2& right);

		friend Vec2 operator*(const Vec2& left, float right);
		friend Vec2 operator*(float left, const Vec2& right);
	};

	class Vec3
	{
	public:
		float x;
		float y;
		float z;

		Vec3(float _x = 0, float _y = 0, float _z = 0);
		Vec3(const Vec2& v, float _z = 0);

		float magnitude();
		Vec3 normalize();

		float dot(const Vec3& v) const;
		Vec3 cross(const Vec3& v) const;

		friend Vec3 operator+(const Vec3& left, const Vec3& right);
		friend Vec3 operator-(const Vec3& left, const Vec3& right);
		friend Vec3 operator*(const Vec3& left, const Vec3& right);
		friend Vec3 operator/(const Vec3& left, const Vec3& right);

		friend Vec3 operator*(const Vec3& left, float right);
		friend Vec3 operator*(float left, const Vec3& right);
	};

	class Vec4
	{
	public:
		float x;
		float y;
		float z;
		float w;

		Vec4(float _x = 0, float _y = 0, float _z = 0, float _w = 0);
		Vec4(const Vec2& v, float _z = 0, float _w = 0);
		Vec4(const Vec3& v, float _w = 0);

		float magnitude();
		Vec4 normalize();

		friend Vec4 operator+(const Vec4& left, const Vec4& right);
		friend Vec4 operator-(const Vec4& left, const Vec4& right);
		friend Vec4 operator*(const Vec4& left, const Vec4& right);
		friend Vec4 operator/(const Vec4& left, const Vec4& right);

		friend Vec4 operator*(const Vec4& left, float right);
		friend Vec4 operator*(float left, const Vec4& right);

		friend Vec4 operator*(const Vec4& left, const Mat4& right);
	};
}
