#include "RasterizerWireframe.h"
#include "RasterState.h"
#include "FrameBuffer.h"

namespace SoftRenderer
{
    void RasterizerWireframe::drawLine(FrameBuffer& fbo, const Vec4& begin, const Vec4& end, const Vec4& color)
    {
        float beginX = clamp(begin.x, 0.0f, fbo.size.x);
        float beginY = clamp(begin.y, 0.0f, fbo.size.y);
        float endX = clamp(end.x, 0.0f, fbo.size.x);
        float endY = clamp(end.y, 0.0f, fbo.size.y);
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

    void RasterizerWireframe::drawTriangle(FrameBuffer& fbo, std::unique_ptr<Shader>& shader, ShaderV2F v0, ShaderV2F v1, ShaderV2F v2, const RasterState& rState)
    {
        if ((rState.rasterMode & RasterMode::Wireframe) == RasterMode::None)
        {
            return;
        }

        // Í¸ÊÓ³ý·¨
        v0.position.x /= v0.position.w;
        v0.position.y /= v0.position.w;
        v0.position.z /= v0.position.w;
        v0.position.w = 1.0f / v0.position.w;

        v1.position.x /= v1.position.w;
        v1.position.y /= v1.position.w;
        v1.position.z /= v1.position.w;
        v1.position.w = 1.0f / v1.position.w;

        v2.position.x /= v2.position.w;
        v2.position.y /= v2.position.w;
        v2.position.z /= v2.position.w;
        v2.position.w = 1.0f / v2.position.w;

        // viewport
        v0.position.x = (v0.position.x + 1) * 0.5f * fbo.size.x;
        v0.position.y = (v0.position.y + 1) * 0.5f * fbo.size.y;

        v1.position.x = (v1.position.x + 1) * 0.5f * fbo.size.x;
        v1.position.y = (v1.position.y + 1) * 0.5f * fbo.size.y;

        v2.position.x = (v2.position.x + 1) * 0.5f * fbo.size.x;
        v2.position.y = (v2.position.y + 1) * 0.5f * fbo.size.y;

        drawLine(fbo, v0.position, v1.position, Vec4(1, 1, 1, 1));
        drawLine(fbo, v1.position, v2.position, Vec4(1, 1, 1, 1));
        drawLine(fbo, v2.position, v0.position, Vec4(1, 1, 1, 1));
    }
}
