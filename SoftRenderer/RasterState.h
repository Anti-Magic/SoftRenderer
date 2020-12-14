#pragma once

#include "Vec.h"

namespace SoftRenderer
{
	enum class RasterMode : int
	{
		None = 0,
		Fill = 1,
		Wireframe = 2,
		All = 3,
	};

	RasterMode operator&(RasterMode a, RasterMode b);
	RasterMode operator|(RasterMode a, RasterMode b);
	RasterMode operator~(RasterMode a);
	RasterMode operator&=(RasterMode& a, RasterMode b);
	RasterMode operator|=(RasterMode& a, RasterMode b);

	enum class FaceCulling
	{
		None,
		Back,
		Front,
	};

	class RasterState
	{
	public:
		bool enableDepthTest;
		bool enableDepthWrite;
		bool enableClipping;
		RasterMode rasterMode;
		FaceCulling cullType;

		RasterState();
		bool depthTestFunc(float src, float dst) const;
		Vec4 alphaBlendFunc(const Vec4& src, const Vec4& dst) const;
	};
}
