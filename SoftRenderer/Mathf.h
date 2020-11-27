#pragma once

#include <cmath>
#include <algorithm>
#include "Vec.h"
#include "Mat.h"
#include "Quat.h"

namespace SoftRenderer
{
	const float PI = 3.1415926535f;

	template<typename T>
	static float clamp(const T& value, const T& min = 0, float max = 1)
	{
		return std::max(min, std::min(value, max));
	}

	template<typename T>
	static T lerp(const T& min, const T& max, float gradient)
	{
		return min + (max - min) * gradient;
	}

	static Vec3 reflect(const Vec3& v, const Vec3& n)
	{
		return v - 2 * n * v.dot(n);
	}

	static Vec3 refract(const Vec3& v, const Vec3& n, float eta)
	{
		float dt = v.dot(n);
		float k = 1 - eta * eta * (1 - dt * dt);
		if (k < 0) {
			return Vec3(0, 0, 0);
		}
		return (v - n * dt) * eta - n * std::sqrt(k);
	}

	// cos为入射角的余弦，refractive为入射介质的折射率
	// 例如光线从真空射向水，水的折射率为1.33，cos为光线和法线的锐角
	static float fresnelSchlick(float cos, float refractive)
	{
		float r0 = (1 - refractive) / (1 + refractive);
		float f = r0 * r0;
		return f + (1 - f) * std::pow(1 - cos, 5);
	}
}
