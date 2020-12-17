#include "RasterPipeline.h"
#include "FrameBuffer.h"
#include "Shader.h"
#include "Mesh.h"
#include "ClippingPlane.h"
#include "RasterizerScanline.h"
#include "RasterizerHalfSpace.h"
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

    void RasterPipeline::drawTriangle(FrameBuffer& fbo, std::unique_ptr<Shader>& shader, const Vertex& v0Raw, const Vertex& v1Raw, const Vertex& v2Raw, const RasterState& rState)
    {
        ShaderV2F v0 = shader->vert(v0Raw);
        ShaderV2F v1 = shader->vert(v1Raw);
        ShaderV2F v2 = shader->vert(v2Raw);

        std::vector<ShaderV2F> clippedVerts = Clipping(v0, v1, v2, rState);

        for (auto& v : clippedVerts)
        {
            PerspectiveDivision(v);
            PerspectiveCorrect(v);
            ViewPortTransform(v, fbo.size);
        }

        for (int i = 0; i + 1 < clippedVerts.size(); i += 2)
        {
            auto& cv0 = clippedVerts[i];
            auto& cv1 = clippedVerts[i + 1];
            auto& cv2 = clippedVerts[(i + 2) % clippedVerts.size()];
            if (Culling(cv0, cv1, cv2, rState))
            {
                continue;
            }
            if ((rState.rasterMode & RasterMode::Fill) != RasterMode::None)
            {
				if (rState.rasterMethod == RasterMethod::HalfSpace)
				{
					RasterizerHalfSpace::drawTriangle(fbo, shader, cv0, cv1, cv2, rState);
				}
				else
				{
					RasterizerScanline::drawTriangle(fbo, shader, cv0, cv1, cv2, rState);
				}
            }
            if ((rState.rasterMode & RasterMode::Wireframe) != RasterMode::None)
            {
                RasterizerWireframe::drawTriangle(fbo, shader, clippedVerts[0], clippedVerts[1], clippedVerts[2], rState);
            }
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
            return std::vector<ShaderV2F> { };
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
        v.position.w = 1.0f / v.position.w;
        v.perspectiveCorrect(v.position.w);
    }

    void RasterPipeline::ViewPortTransform(ShaderV2F& v0, const Vec2& size)
    {
        v0.position.x = (v0.position.x + 1) * 0.5f * size.x;
        v0.position.y = (v0.position.y + 1) * 0.5f * size.y;
    }
}
