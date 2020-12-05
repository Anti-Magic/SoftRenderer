#include "RasterizerScanLine.h"
#include "FrameBuffer.h"
#include "Shader.h"
#include "Mesh.h"

namespace SoftRenderer 
{
    void RasterizerScanline::drawTriangle(FrameBuffer& fbo, std::unique_ptr<Shader>& shader, ShaderV2F& v0, ShaderV2F& v1, ShaderV2F& v2)
    {
        // 透视除法
        v0.position.x /= v0.position.w;
        v0.position.y /= v0.position.w;
        v0.position.z /= v0.position.w;

        v1.position.x /= v1.position.w;
        v1.position.y /= v1.position.w;
        v1.position.z /= v1.position.w;

        v2.position.x /= v2.position.w;
        v2.position.y /= v2.position.w;
        v2.position.z /= v2.position.w;

        // 透视插值
        v0.mul(1.0f / v0.position.w);
        v1.mul(1.0f / v1.position.w);
        v2.mul(1.0f / v2.position.w);

        // viewport
        v0.position.x = (v0.position.x + 1) * 0.5f * fbo.size.x;
        v0.position.y = (v0.position.y + 1) * 0.5f * fbo.size.y;

        v1.position.x = (v1.position.x + 1) * 0.5f * fbo.size.x;
        v1.position.y = (v1.position.y + 1) * 0.5f * fbo.size.y;

        v2.position.x = (v2.position.x + 1) * 0.5f * fbo.size.x;
        v2.position.y = (v2.position.y + 1) * 0.5f * fbo.size.y;

        // y sort
        if (v0.position.y > v1.position.y) std::swap(v0, v1);
        if (v1.position.y > v2.position.y) std::swap(v1, v2);
        if (v0.position.y > v1.position.y) std::swap(v0, v1);

        if (v0.position.y == v1.position.y)
        {
            if (v0.position.x > v1.position.x)
            {
                std::swap(v0, v1);
            }
            drawFlatBotTriangle(fbo, shader, v0, v1, v2);
        }
        else if (v1.position.y == v2.position.y)
        {
            if (v1.position.x > v2.position.x)
            {
                std::swap(v1, v2);
            }
            drawFlatTopTriangle(fbo, shader, v0, v1, v2);
        }
        else
        {
            float t = (v1.position.y - v0.position.y) / (v2.position.y - v0.position.y);
            Vec4 pEx = lerp(v0.position, v2.position, t);

            if (v1.position.x > pEx.x)
            {
                std::swap(v1, v2);
                drawFlatTopTriangle(fbo, shader, v0, v1, v2);
                std::swap(v1, v2);
            }
            else
            {
                drawFlatTopTriangle(fbo, shader, v0, v1, v2);
            }

            if (pEx.x > v1.position.x)
            {
                std::swap(v0, v1);
            }
            drawFlatBotTriangle(fbo, shader, v0, v1, v2);
        }
    }

    void RasterizerScanline::drawFlatTopTriangle(FrameBuffer& fbo, std::unique_ptr<Shader>& shader, const ShaderV2F& v0, const ShaderV2F& v1, const ShaderV2F& v2)
    {
        //  1-------2
        //   \     /
        //    \   /
        //     \ /
        //      0
        float yel = floor(v1.position.y);
        float yer = floor(v2.position.y);
        float ys = floor(v0.position.y);
        float ye = std::min(yel, yer);
        int loopLow = clamp(ys, 0.0f, fbo.size.y);
        int loopHigh = clamp(ye, 0.0f, fbo.size.y);
        for (int y = loopLow; y < loopHigh; y++)
        {
            ShaderV2F vl = ShaderV2F::lerpV2F(v0, v1, (y - ys) / (yel - ys));
            ShaderV2F vr = ShaderV2F::lerpV2F(v0, v2, (y - ys) / (yer - ys));
            drawScanline(fbo, shader, vl, vr, y);
        }

        //drawLine(fbo, v0.position, v1.position);
        //drawLine(fbo, v1.position, v2.position);
        //drawLine(fbo, v2.position, v0.position);
    }

    void RasterizerScanline::drawFlatBotTriangle(FrameBuffer& fbo, std::unique_ptr<Shader>& shader, const ShaderV2F& v0, const ShaderV2F& v1, const ShaderV2F& v2)
    {
        //      2
        //     / \
        //    /   \
        //   /     \
        //  0-------1
        float ysl = floor(v0.position.y);
        float ysr = floor(v1.position.y);
        float ys = std::max(ysl, ysr);
        float ye = floor(v2.position.y);
        int loopLow = clamp(ys, 0.0f, fbo.size.y);
        int loopHigh = clamp(ye, 0.0f, fbo.size.y);
        for (int y = loopLow; y < loopHigh; y++)
        {
            ShaderV2F vl = ShaderV2F::lerpV2F(v2, v0, (ye - y) / (ye - ysl));
            ShaderV2F vr = ShaderV2F::lerpV2F(v2, v1, (ye - y) / (ye - ysr));
            drawScanline(fbo, shader, vl, vr, y);
        }

        //drawLine(fbo, v0.position, v1.position);
        //drawLine(fbo, v1.position, v2.position);
        //drawLine(fbo, v2.position, v0.position);
    }

    void RasterizerScanline::drawScanline(FrameBuffer& fbo, std::unique_ptr<Shader>& shader, const ShaderV2F& vl, const ShaderV2F& vr, int y)
    {
        float xs = floor(vl.position.x);
        float xe = floor(vr.position.x);
        int loopLow = clamp(xs, 0.0f, fbo.size.x);
        int loopHigh = clamp(xe, 0.0f, fbo.size.x);
        for (int x = loopLow; x < loopHigh; x++)
        {
            float t = (x - xs) / (xe - xs);
            ShaderV2F vm = ShaderV2F::lerpV2F(vl, vr, t);
            if (depthTest(vm.position.z, fbo.getDepth(x, y)))
            {
                vm.mul(vm.position.w);
                Vec4 color = shader->frag(vm);
                color = alphaBlend(color, fbo.getColor(x, y));
                fbo.setColor(x, y, color);
                fbo.setDepth(x, y, vm.position.z);
            }
        }
    }

    bool RasterizerScanline::depthTest(float src, float dst)
    {
        //return src <= 1 && src >= dst;
        return src <= dst;
    }

    Vec4 RasterizerScanline::alphaBlend(const Vec4& src, const Vec4& dst)
    {
        float alpha = src.w;
        return Vec4(src.x * alpha + dst.x * (1 - alpha), src.y * alpha + dst.y * (1 - alpha), src.z * alpha + dst.z * (1 - alpha), 1);
    }
}
