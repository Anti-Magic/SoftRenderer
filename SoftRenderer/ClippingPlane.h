#pragma once

#include "Vec.h"

namespace SoftRenderer
{
	class ClippingPlane
	{
	public:
		Vec3 normal;
		float distance;

		ClippingPlane(const Vec3& _normal, float _distance);
		float DistanceFromPoint4(const Vec4& p);
	};
}
