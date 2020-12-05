#include "ClippingPlane.h"

namespace SoftRenderer
{
	ClippingPlane::ClippingPlane(const Vec3& _normal, float _distance)
		: normal(_normal)
		, distance(_distance)
	{
	}

	float ClippingPlane::DistanceFromPoint4(const Vec4& p)
	{
		return p.x * normal.x + p.y * normal.y + p.z * normal.z + p.w * distance;
	}
}
