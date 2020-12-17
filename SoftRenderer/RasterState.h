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

	enum class RasterMethod : int
	{
		HalfSpace = 0,
		Scanline = 1,
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
		RasterMethod rasterMethod;
		FaceCulling cullType;

		RasterState();
		bool DepthTestFunc(float src, float dst) const;
		Vec4 AlphaBlendFunc(const Vec4& src, const Vec4& dst) const;
	};
}
