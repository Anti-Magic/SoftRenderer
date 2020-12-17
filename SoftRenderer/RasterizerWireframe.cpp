#include "RasterizerWireframe.h"
#include "RasterState.h"
#include "FrameBuffer.h"

namespace SoftRenderer
{
    void RasterizerWireframe::drawTriangle(FrameBuffer& fbo, std::unique_ptr<Shader>& shader, ShaderV2F v0, ShaderV2F v1, ShaderV2F v2, const RasterState& rState)
    {
		drawLineLerp(fbo, v0.position, v1.position, Vec4(1, 1, 1, 1));
		drawLineLerp(fbo, v1.position, v2.position, Vec4(1, 1, 1, 1));
		drawLineLerp(fbo, v2.position, v0.position, Vec4(1, 1, 1, 1));

        //drawLineBresenham(fbo, v0.position, v1.position, Vec4(1, 1, 1, 1));
        //drawLineBresenham(fbo, v1.position, v2.position, Vec4(1, 1, 1, 1));
        //drawLineBresenham(fbo, v2.position, v0.position, Vec4(1, 1, 1, 1));
    }

	void RasterizerWireframe::drawLineLerp(FrameBuffer& fbo, const Vec4& begin, const Vec4& end, const Vec4& color)
	{
		int minX = clamp(std::min(begin.x, end.x), 0.0f, fbo.size.x - 1);
		int maxX = clamp(std::max(begin.x, end.x), 0.0f, fbo.size.x - 1);
		int minY = clamp(std::min(begin.y, end.y), 0.0f, fbo.size.y - 1);
		int maxY = clamp(std::max(begin.y, end.y), 0.0f, fbo.size.y - 1);
		if (maxX - minX > maxY - minY)
		{
			for (int x = minX; x < maxX; x++)
			{
				Vec4 p = lerp(begin, end, (x - begin.x) / (end.x - begin.x));
				fbo.setColor(p.x, p.y, color);
			}
		}
		else
		{
			for (int y = minY; y < maxY; y++)
			{
				Vec4 p = lerp(begin, end, (y - begin.y) / (end.y - begin.y));
				fbo.setColor(p.x, p.y, color);
			}
		}
	}

	void RasterizerWireframe::drawLineBresenham(FrameBuffer& fbo, const Vec4& begin, const Vec4& end, const Vec4& color)
	{
		float beginX = clamp(begin.x, 0.0f, fbo.size.x - 1);
		float beginY = clamp(begin.y, 0.0f, fbo.size.y - 1);
		float endX = clamp(end.x, 0.0f, fbo.size.x - 1);
		float endY = clamp(end.y, 0.0f, fbo.size.y - 1);
		int dx = floor(endX) - floor(beginX);
		int dy = floor(endY) - floor(beginY);
		int ux = (dx >= 0) ? 1 : -1;
		int uy = (dy >= 0) ? 1 : -1;
		int x = floor(beginX);
		int y = floor(beginY);
		int endx = floor(endX);
		int endy = floor(endY);
		int eps = 0;
		dx = abs(dx);
		dy = abs(dy);
		if (dx > dy)
		{
			for (; x != endx; x += ux)
			{
				if (x >= 0 && x < fbo.size.x && y >= 0 && y < fbo.size.y)
				{
					fbo.setColor(x, y, color);
				}

				eps += dy;
				if ((eps << 1) >= dx)
				{
					y += uy;
					eps -= dx;
				}
			}
		}
		else
		{
			for (; y != endy; y += uy)
			{
				if (x >= 0 && x < fbo.size.x && y >= 0 && y < fbo.size.y)
				{
					fbo.setColor(x, y, color);
				}

				eps += dx;
				if ((eps << 1) >= dy)
				{
					x += ux;
					eps -= dy;
				}
			}
		}
	}
}
