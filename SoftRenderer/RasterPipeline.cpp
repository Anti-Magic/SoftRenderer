#include "RasterPipeline.h"
#include "FrameBuffer.h"
#include "Shader.h"
#include "Mesh.h"
#include "ClippingPlane.h"
#include "RasterizerScanline.h"
#include "RasterizerBaryCoord.h"
#include "RasterizerWireframe.h"
#include "RasterState.h"

namespace SoftRenderer 
{
    void RasterPipeline::drawTriangles(FrameBuffer& fbo, std::unique_ptr<Shader>& shader, const Mesh& mesh, const RasterState& rState)
    {
        if (mesh.indices.size() > 0)
        {
            for (size_t i = 0; i < mesh.indices.size(); i += 3)
            {
                drawTriangle(fbo, shader, mesh.vertices[mesh.indices[i]], mesh.vertices[mesh.indices[i + 1]], mesh.vertices[mesh.indices[i + 2]], rState);
            }
        }
        else
        {
            for (size_t i = 0; i < mesh.vertices.size(); i += 3)
            {
                drawTriangle(fbo, shader, mesh.vertices[i], mesh.vertices[i + 1], mesh.vertices[i + 2], rState);
            }
        }
    }

    void RasterPipeline::drawTriangle(FrameBuffer& fbo, std::unique_ptr<Shader>& shader, const Vertex& v0, const Vertex& v1, const Vertex& v2, const RasterState& rState)
    {
        ShaderV2F vf0 = shader->vert(v0);
        ShaderV2F vf1 = shader->vert(v1);
        ShaderV2F vf2 = shader->vert(v2);

        if (!rState.enableClipping)
        {
            // 无需裁剪
            RasterizerScanline::drawTriangle(fbo, shader, vf0, vf1, vf2, rState);
            RasterizerWireframe::drawTriangle(fbo, shader, vf0, vf1, vf2, rState);
            return;
        }

        // 近面裁剪
        ClippingPlane near(Vec3(0, 0, 1), 0);
        float d0 = near.DistanceFromPoint4(vf0.position);
        float d1 = near.DistanceFromPoint4(vf1.position);
        float d2 = near.DistanceFromPoint4(vf2.position);
        int insideCount = 0;
        if (d0 >= 0) insideCount += 1;
        if (d1 >= 0) insideCount += 1;
        if (d2 >= 0) insideCount += 1;

        if (insideCount == 0)
        {
            // 直接剔除
        }
        else if (insideCount == 1)
        {
            // 裁成1个新的三角形
            if (d0 >= 0)
            {
                ShaderV2F vf1s = ShaderV2F::lerp(vf0, vf1, d0 / (d0 - d1));
                ShaderV2F vf2s = ShaderV2F::lerp(vf0, vf2, d0 / (d0 - d2));
                RasterizerScanline::drawTriangle(fbo, shader, vf0, vf1s, vf2s, rState);
                RasterizerWireframe::drawTriangle(fbo, shader, vf0, vf1s, vf2s, rState);
            }
            else if (d1 >= 0)
            {
                ShaderV2F vf0s = ShaderV2F::lerp(vf1, vf0, d1 / (d1 - d0));
                ShaderV2F vf2s = ShaderV2F::lerp(vf1, vf2, d1 / (d1 - d2));
                RasterizerScanline::drawTriangle(fbo, shader, vf0s, vf1, vf2s, rState);
                RasterizerWireframe::drawTriangle(fbo, shader, vf0s, vf1, vf2s, rState);
            }
            else if (d2 >= 0)
            {
                ShaderV2F vf0s = ShaderV2F::lerp(vf2, vf0, d2 / (d2 - d0));
                ShaderV2F vf1s = ShaderV2F::lerp(vf2, vf1, d2 / (d2 - d1));
                RasterizerScanline::drawTriangle(fbo, shader, vf0s, vf1s, vf2, rState);
                RasterizerWireframe::drawTriangle(fbo, shader, vf0s, vf1s, vf2, rState);
            }
        }
        else if (insideCount == 2)
        {
            if (d0 < 0)
            {
                ShaderV2F vf1s = ShaderV2F::lerp(vf0, vf1, d0 / (d0 - d1));
                ShaderV2F vf2s = ShaderV2F::lerp(vf0, vf2, d0 / (d0 - d2));
                RasterizerScanline::drawTriangle(fbo, shader, vf1s, vf1, vf2, rState);
                RasterizerScanline::drawTriangle(fbo, shader, vf2, vf2s, vf1s, rState);
                RasterizerWireframe::drawTriangle(fbo, shader, vf1s, vf1, vf2, rState);
                RasterizerWireframe::drawTriangle(fbo, shader, vf2, vf2s, vf1s, rState);
            }
            else if (d1 < 0)
            {
                ShaderV2F vf0s = ShaderV2F::lerp(vf1, vf0, d1 / (d1 - d0));
                ShaderV2F vf2s = ShaderV2F::lerp(vf1, vf2, d1 / (d1 - d2));
                RasterizerScanline::drawTriangle(fbo, shader, vf2s, vf2, vf0, rState);
                RasterizerScanline::drawTriangle(fbo, shader, vf0, vf0s, vf2s, rState);
                RasterizerWireframe::drawTriangle(fbo, shader, vf2s, vf2, vf0, rState);
                RasterizerWireframe::drawTriangle(fbo, shader, vf0, vf0s, vf2s, rState);
            }
            else if (d2 < 0)
            {
                ShaderV2F vf0s = ShaderV2F::lerp(vf2, vf0, d2 / (d2 - d0));
                ShaderV2F vf1s = ShaderV2F::lerp(vf2, vf1, d2 / (d2 - d1));
                RasterizerScanline::drawTriangle(fbo, shader, vf0s, vf0, vf1, rState);
                RasterizerScanline::drawTriangle(fbo, shader, vf1, vf1s, vf0s, rState);
                RasterizerWireframe::drawTriangle(fbo, shader, vf0s, vf0, vf1, rState);
                RasterizerWireframe::drawTriangle(fbo, shader, vf1, vf1s, vf0s, rState);
            }
        }
        else
        {
            // 无需裁剪
            RasterizerScanline::drawTriangle(fbo, shader, vf0, vf1, vf2, rState);
            RasterizerWireframe::drawTriangle(fbo, shader, vf0, vf1, vf2, rState);
        }
    }
    
    void RasterPipeline::PerspectiveDivision(ShaderV2F& v)
    {
        v.position.x /= v.position.w;
        v.position.y /= v.position.w;
        v.position.z /= v.position.w;
    }

    std::vector<ShaderV2F> RasterPipeline::Clipping(const ShaderV2F& v0, const ShaderV2F& v1, const ShaderV2F& v2, const RasterState& rState)
    {
        if (!rState.enableClipping)
        {
            return std::vector<ShaderV2F> {v0, v1, v2};
        }

        // 近面裁剪
        ClippingPlane near(Vec3(0, 0, 1), 0);
        float d0 = near.DistanceFromPoint4(v0.position);
        float d1 = near.DistanceFromPoint4(v1.position);
        float d2 = near.DistanceFromPoint4(v2.position);
        int insideCount = 0;
        if (d0 >= 0) insideCount += 1;
        if (d1 >= 0) insideCount += 1;
        if (d2 >= 0) insideCount += 1;

        if (insideCount == 0)
        {
            // 直接剔除
        }
        else if (insideCount == 1)
        {
            // 裁成1个新的三角形
            if (d0 >= 0)
            {
                ShaderV2F v1s = ShaderV2F::lerp(v0, v1, d0 / (d0 - d1));
                ShaderV2F v2s = ShaderV2F::lerp(v0, v2, d0 / (d0 - d2));
                return std::vector<ShaderV2F> {v0, v1s, v2s};
            }
            else if (d1 >= 0)
            {
                ShaderV2F v0s = ShaderV2F::lerp(v1, v0, d1 / (d1 - d0));
                ShaderV2F v2s = ShaderV2F::lerp(v1, v2, d1 / (d1 - d2));
                return std::vector<ShaderV2F> {v0s, v1, v2s};
            }
            else if (d2 >= 0)
            {
                ShaderV2F v0s = ShaderV2F::lerp(v2, v0, d2 / (d2 - d0));
                ShaderV2F v1s = ShaderV2F::lerp(v2, v1, d2 / (d2 - d1));
                return std::vector<ShaderV2F> {v0s, v1s, v2};
            }
        }
        else if (insideCount == 2)
        {
            if (d0 < 0)
            {
                ShaderV2F v1s = ShaderV2F::lerp(v0, v1, d0 / (d0 - d1));
                ShaderV2F v2s = ShaderV2F::lerp(v0, v2, d0 / (d0 - d2));
                return std::vector<ShaderV2F> {v1s, v1, v2, v2s};
            }
            else if (d1 < 0)
            {
                ShaderV2F v0s = ShaderV2F::lerp(v1, v0, d1 / (d1 - d0));
                ShaderV2F v2s = ShaderV2F::lerp(v1, v2, d1 / (d1 - d2));
                return std::vector<ShaderV2F> {v2s, v2, v0, v0s};
            }
            else if (d2 < 0)
            {
                ShaderV2F v0s = ShaderV2F::lerp(v2, v0, d2 / (d2 - d0));
                ShaderV2F v1s = ShaderV2F::lerp(v2, v1, d2 / (d2 - d1));
                return std::vector<ShaderV2F> {v0s, v0, v1, v1s};
            }
        }
        else
        {
            // 无需裁剪
            return std::vector<ShaderV2F> {v0, v1, v2};
        }
    }

    bool RasterPipeline::Culling(const ShaderV2F& v0, const ShaderV2F& v1, const ShaderV2F& v2, const RasterState& rState)
    {
        // backface culling
        if (rState.cullType == FaceCulling::Back)
        {
            Vec2 dir01(v1.position.x - v0.position.x, v1.position.y - v0.position.y);
            Vec2 dir02(v2.position.x - v0.position.x, v2.position.y - v0.position.y);
            if (dir01.x * dir02.y - dir01.y * dir02.x > 0)
            {
                return true;
            }
        }
        return false;
    }

    void RasterPipeline::PerspectiveCorrect(ShaderV2F& v)
    {
        // 透视插值
        v.position.w = 1.0f / v.position.w;
        v.perspectiveCorrect(v.position.w);
    }

    void RasterPipeline::ViewPortTransform(ShaderV2F& v0, const Vec2& size)
    {
        // viewport
        v0.position.x = (v0.position.x + 1) * 0.5f * size.x;
        v0.position.y = (v0.position.y + 1) * 0.5f * size.y;
    }
}
