#include "RasterState.h"

namespace SoftRenderer
{
	RasterMode operator&(RasterMode a, RasterMode b)
	{
		return static_cast<RasterMode>(static_cast<int>(a) & static_cast<int>(b));
	}

	RasterMode operator|(RasterMode a, RasterMode b)
	{
		return static_cast<RasterMode>(static_cast<int>(a) | static_cast<int>(b));
	}

	RasterMode operator~(RasterMode a)
	{
		return static_cast<RasterMode>(~static_cast<int>(a));
	}

	RasterMode operator&=(RasterMode& a, RasterMode b)
	{
		a = a & b;
		return a;
	}

	RasterMode operator|=(RasterMode& a, RasterMode b)
	{
		a = a | b;
		return a;
	}

	RasterState::RasterState()
		: enableDepthTest(true)
		, enableDepthWrite(true)
		, enableClipping(true)
		, rasterMode(RasterMode::Fill)
		, cullType(FaceCulling::Back)
	{
	}

	bool RasterState::depthTestFunc(float src, float dst) const
	{
		//return src <= 1 && src >= dst;
		return src <= dst;
	}

	Vec4 RasterState::alphaBlendFunc(const Vec4& src, const Vec4& dst) const
	{
		float alpha = src.w;
		return src * alpha + dst * (1 - alpha);
	}
}
