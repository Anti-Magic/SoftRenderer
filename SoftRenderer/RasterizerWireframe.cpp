#include "RasterizerWireframe.h"
#include "FrameBuffer.h"

namespace SoftRenderer
{
    void RasterizerWireframe::drawLine(FrameBuffer& fbo, const Vec4& begin, const Vec4& end, const Vec4& color)
    {
        int dx = floor(end.x) - floor(begin.x);
        int dy = floor(end.y) - floor(begin.y);
        int ux = (dx >= 0) ? 1 : -1;
        int uy = (dy >= 0) ? 1 : -1;
        int x = floor(begin.x);
        int y = floor(begin.y);
        int endx = floor(end.x);
        int endy = floor(end.y);
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

    void RasterizerWireframe::drawTriangle(FrameBuffer& fbo, std::unique_ptr<Shader>& shader, ShaderV2F& v0, ShaderV2F& v1, ShaderV2F& v2)
    {
        drawLine(fbo, v0.position, v1.position, Vec4(1, 1, 1, 1));
        drawLine(fbo, v1.position, v2.position, Vec4(1, 1, 1, 1));
        drawLine(fbo, v2.position, v0.position, Vec4(1, 1, 1, 1));
    }
}
