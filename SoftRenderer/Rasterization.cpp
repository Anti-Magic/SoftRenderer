#include "Rasterization.h"
#include "FrameBuffer.h"
#include "Shader.h"
#include "Mesh.h"

namespace SoftRenderer
{
    void Rasterization::drawLine(FrameBuffer& fbo, const Vec4& begin, const Vec4& end)
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
                    fbo.setColor(x, y, Vec4(1, 0, 0, 1));
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
                    fbo.setColor(x, y, Vec4(1, 0, 0, 1));
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

    void Rasterization::drawTriangles(FrameBuffer& fbo, std::unique_ptr<Shader>& shader, const Mesh& mesh)
    {
        RasterizationParam param(fbo, shader);
        if (mesh.indices.size() > 0)
        {
            for (size_t i = 0; i < mesh.indices.size(); i += 3)
            {
                drawTriangle(param, mesh.vertices[mesh.indices[i]], mesh.vertices[mesh.indices[i + 1]], mesh.vertices[mesh.indices[i + 2]]);
            }
        }
        else
        {
            for (size_t i = 0; i < mesh.vertices.size(); i += 3)
            {
                drawTriangle(param, mesh.vertices[i], mesh.vertices[i + 1], mesh.vertices[i + 2]);
            }
        }
    }

    bool Rasterization::cvvCulling(const ShaderV2F& v)
    {
        float w = v.position.w;
        if (v.position.x < -w || v.position.x > w)
        {
            return true;
        }

        if (v.position.y < -w || v.position.y > w)
        {
            return true;
        }

        if (v.position.z < 0 || v.position.z > w)
        {
            return true;
        }

        return false;
    }

    void Rasterization::drawTriangle(RasterizationParam& param, const Vertex& v0, const Vertex& v1, const Vertex& v2)
    {
        param.shader->vert(v0, param.v[0]);
        param.shader->vert(v1, param.v[1]);
        param.shader->vert(v2, param.v[2]);
        for (int i = 0; i < 3; i++)
        {
            ShaderV2F& v = param.v[i];
            v.position.x /= v.position.w;
            v.position.y /= v.position.w;
            v.position.z /= v.position.w;
            v.position.w = 1.0f / v.position.w;
            v.position.x = (v.position.x + 1) * 0.5f * param.fbo.size.x;
            v.position.y = (v.position.y + 1) * 0.5f * param.fbo.size.y;
            v.mul(v.position.w);
        }

        if (param.v[2].position.y < param.v[1].position.y)
        {
            std::swap(param.v[1], param.v[2]);
        }

        if (param.v[1].position.y < param.v[0].position.y)
        {
            std::swap(param.v[0], param.v[1]);
        }

        if (param.v[2].position.y < param.v[1].position.y)
        {
            std::swap(param.v[1], param.v[2]);
        }

        if (param.v[0].position.y == param.v[1].position.y)
        {
            if (param.v[0].position.x > param.v[1].position.x)
            {
                std::swap(param.v[0], param.v[1]);
            }
            drawFlatBotTriangle(param);
        }
        else if (param.v[1].position.y == param.v[2].position.y)
        {
            if (param.v[1].position.x > param.v[2].position.x)
            {
                std::swap(param.v[1], param.v[2]);
            }
            drawFlatTopTriangle(param);
        }
        else
        {
            ShaderV2F vd = param.v[0];
            ShaderV2F vm = param.v[1];
            ShaderV2F vu = param.v[2];

            float t = (param.v[1].position.y - param.v[0].position.y) / (param.v[2].position.y - param.v[0].position.y);
            Vec4 pEx = lerp(param.v[0].position, param.v[2].position, t);

            if (param.v[1].position.x > pEx.x)
            {
                std::swap(param.v[1], param.v[2]);
            }
            drawFlatTopTriangle(param);

            param.v[0] = vd;
            param.v[1] = vm;
            param.v[2] = vu;
            if (pEx.x > param.v[1].position.x)
            {
                std::swap(param.v[0], param.v[1]);
            }
            drawFlatBotTriangle(param);
        }
    }

    void Rasterization::drawFlatTopTriangle(RasterizationParam& param)
    {
        //  1-------2
        //   \     /
        //    \   /
        //     \ /
        //      0
        float yel = floor(param.v[1].position.y);
        float yer = floor(param.v[2].position.y);
        float ys = floor(param.v[0].position.y);
        float ye = std::min(yel, yer);
        int loopLow = clamp(ys, 0.0f, param.fbo.size.y);
        int loopHigh = clamp(ye, 0.0f, param.fbo.size.y);
        for (int y = loopLow; y < loopHigh; y++)
        {
            param.vfl.fromLerp(param.v[0], param.v[1], (y - ys) / (yel - ys));
            param.vfr.fromLerp(param.v[0], param.v[2], (y - ys) / (yer - ys));
            drawScanLine(param, y);
        }

        drawLine(param.fbo, param.v[0].position, param.v[1].position);
        drawLine(param.fbo, param.v[1].position, param.v[2].position);
        drawLine(param.fbo, param.v[2].position, param.v[0].position);
    }

    void Rasterization::drawFlatBotTriangle(RasterizationParam& param)
    {
        //      2
        //     / \
        //    /   \
        //   /     \
        //  0-------1
        float ysl = floor(param.v[0].position.y);
        float ysr = floor(param.v[1].position.y);
        float ys = std::max(ysl, ysr);
        float ye = floor(param.v[2].position.y);
        int loopLow = clamp(ys, 0.0f, param.fbo.size.y);
        int loopHigh = clamp(ye, 0.0f, param.fbo.size.y);
        for (int y = loopLow; y < loopHigh; y++)
        {
            param.vfl.fromLerp(param.v[2], param.v[0], (ye - y) / (ye - ysl));
            param.vfr.fromLerp(param.v[2], param.v[1], (ye - y) / (ye - ysr));
            drawScanLine(param, y);
        }

        drawLine(param.fbo, param.v[0].position, param.v[1].position);
        drawLine(param.fbo, param.v[1].position, param.v[2].position);
        drawLine(param.fbo, param.v[2].position, param.v[0].position);
    }

    void Rasterization::drawScanLine(RasterizationParam& param, int y)
    {
        float xs = floor(param.vfl.position.x);
        float xe = floor(param.vfr.position.x);
        int loopLow = clamp(xs, 0.0f, param.fbo.size.x);
        int loopHigh = clamp(xe, 0.0f, param.fbo.size.x);
        for (int x = loopLow; x < loopHigh; x++)
        {
            float t = (x - xs) / (xe - xs);
            param.vfm.fromLerp(param.vfl, param.vfr, t);
            float depth = param.vfm.position.w;
            if (depthTest(depth, param.fbo.getDepth(x, y)))
            {
                param.vfm.mul(1 / param.vfm.position.w);
                Vec4 color = param.shader->frag(param.vfm);
                color = alphaBlend(color, param.fbo.getColor(x, y));
                param.fbo.setColor(x, y, color);
                param.fbo.setDepth(x, y, depth);
            }
        }
    }

    bool Rasterization::depthTest(float src, float dst)
    {
        return src <= 1 && src >= dst;
    }

    Vec4 Rasterization::alphaBlend(const Vec4& src, const Vec4& dst)
    {
        float alpha = src.w;
        return Vec4(src.x * alpha + dst.x * (1 - alpha), src.y * alpha + dst.y * (1 - alpha), src.z * alpha + dst.z * (1 - alpha), 1);
    }
}
