#include "RasterizerScanLine.h"
#include "RasterState.h"
#include "FrameBuffer.h"
#include "Shader.h"
#include "Mesh.h"

namespace SoftRenderer 
{
    void RasterizerScanline::drawTriangle(FrameBuffer& fbo, std::unique_ptr<Shader>& shader, ShaderV2F v0, ShaderV2F v1, ShaderV2F v2, const RasterState& rState)
    {
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
            drawFlatBotTriangle(fbo, shader, v0, v1, v2, rState);
        }
        else if (v1.position.y == v2.position.y)
        {
            if (v1.position.x > v2.position.x)
            {
                std::swap(v1, v2);
            }
            drawFlatTopTriangle(fbo, shader, v0, v1, v2, rState);
        }
        else
        {
            float t = (v1.position.y - v0.position.y) / (v2.position.y - v0.position.y);
            Vec4 pEx = lerp(v0.position, v2.position, t);

            if (v1.position.x > pEx.x)
            {
                std::swap(v1, v2);
                drawFlatTopTriangle(fbo, shader, v0, v1, v2, rState);
                std::swap(v1, v2);
            }
            else
            {
                drawFlatTopTriangle(fbo, shader, v0, v1, v2, rState);
            }

            if (pEx.x > v1.position.x)
            {
                std::swap(v0, v1);
            }
            drawFlatBotTriangle(fbo, shader, v0, v1, v2, rState);
        }
    }

    void RasterizerScanline::drawFlatTopTriangle(FrameBuffer& fbo, std::unique_ptr<Shader>& shader, const ShaderV2F& v0, const ShaderV2F& v1, const ShaderV2F& v2, const RasterState& rState)
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
            ShaderV2F vl = ShaderV2F::lerp(v0, v1, (y - ys) / (yel - ys));
            ShaderV2F vr = ShaderV2F::lerp(v0, v2, (y - ys) / (yer - ys));
            drawScanline(fbo, shader, vl, vr, y, rState);
        }
    }

    void RasterizerScanline::drawFlatBotTriangle(FrameBuffer& fbo, std::unique_ptr<Shader>& shader, const ShaderV2F& v0, const ShaderV2F& v1, const ShaderV2F& v2, const RasterState& rState)
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
            ShaderV2F vl = ShaderV2F::lerp(v2, v0, (ye - y) / (ye - ysl));
            ShaderV2F vr = ShaderV2F::lerp(v2, v1, (ye - y) / (ye - ysr));
            drawScanline(fbo, shader, vl, vr, y, rState);
        }
    }

    void RasterizerScanline::drawScanline(FrameBuffer& fbo, std::unique_ptr<Shader>& shader, const ShaderV2F& vl, const ShaderV2F& vr, int y, const RasterState& rState)
    {
        float xs = floor(vl.position.x);
        float xe = floor(vr.position.x);
        int loopLow = clamp(xs, 0.0f, fbo.size.x);
        int loopHigh = clamp(xe, 0.0f, fbo.size.x);
        for (int x = loopLow; x < loopHigh; x++)
        {
            ShaderV2F vm = ShaderV2F::lerp(vl, vr, (x - xs) / (xe - xs));
            float depth = vm.position.z;
            if (!rState.enableDepthTest || rState.depthTestFunc(depth, fbo.getDepth(x, y)))
            {
                vm.perspectiveCorrect(1.0f / vm.position.w);
                Vec4 color = shader->frag(vm);
                color = rState.alphaBlendFunc(color, fbo.getColor(x, y));
                fbo.setColor(x, y, color);
                if (rState.enableDepthWrite)
                {
                    fbo.setDepth(x, y, depth);
                }
            }
        }
    }
}
