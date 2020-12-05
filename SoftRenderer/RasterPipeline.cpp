#include "RasterPipeline.h"
#include "FrameBuffer.h"
#include "Shader.h"
#include "Mesh.h"
#include "ClippingPlane.h"
#include "RasterizerScanline.h"
#include "RasterizerBaryCoord.h"
#include "RasterizerWireframe.h"

namespace SoftRenderer 
{
    void RasterPipeline::drawTriangles(FrameBuffer& fbo, std::unique_ptr<Shader>& shader, const Mesh& mesh)
    {
        if (mesh.indices.size() > 0)
        {
            for (size_t i = 0; i < mesh.indices.size(); i += 3)
            {
                drawTriangle(fbo, shader, mesh.vertices[mesh.indices[i]], mesh.vertices[mesh.indices[i + 1]], mesh.vertices[mesh.indices[i + 2]]);
            }
        }
        else
        {
            for (size_t i = 0; i < mesh.vertices.size(); i += 3)
            {
                drawTriangle(fbo, shader, mesh.vertices[i], mesh.vertices[i + 1], mesh.vertices[i + 2]);
            }
        }
    }

    void RasterPipeline::drawTriangle(FrameBuffer& fbo, std::unique_ptr<Shader>& shader, const Vertex& v0, const Vertex& v1, const Vertex& v2)
    {
        ShaderV2F vf0 = shader->vert(v0);
        ShaderV2F vf1 = shader->vert(v1);
        ShaderV2F vf2 = shader->vert(v2);

        RasterizerScanline::drawTriangle(fbo, shader, vf0, vf1, vf2);
        RasterizerWireframe::drawTriangle(fbo, shader, vf0, vf1, vf2);
        return;

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
                ShaderV2F vf1s = ShaderV2F::lerpV2F(vf0, vf1, d0 / (d0 - d1));
                ShaderV2F vf2s = ShaderV2F::lerpV2F(vf0, vf2, d0 / (d0 - d2));
                RasterizerScanline::drawTriangle(fbo, shader, vf0, vf1s, vf2s);
                RasterizerWireframe::drawTriangle(fbo, shader, vf0, vf1s, vf2s);
            }
            else if (d1 >= 0)
            {
                ShaderV2F vf0s = ShaderV2F::lerpV2F(vf1, vf0, d1 / (d1 - d0));
                ShaderV2F vf2s = ShaderV2F::lerpV2F(vf1, vf2, d1 / (d1 - d2));
                RasterizerScanline::drawTriangle(fbo, shader, vf0s, vf1, vf2s);
                RasterizerWireframe::drawTriangle(fbo, shader, vf0s, vf1, vf2s);
            }
            else if (d2 >= 0)
            {
                ShaderV2F vf0s = ShaderV2F::lerpV2F(vf2, vf0, d2 / (d2 - d0));
                ShaderV2F vf1s = ShaderV2F::lerpV2F(vf2, vf1, d2 / (d2 - d1));
                RasterizerScanline::drawTriangle(fbo, shader, vf0s, vf1s, vf2);
                RasterizerWireframe::drawTriangle(fbo, shader, vf0s, vf1s, vf2);
            }
        }
        else if (insideCount == 2)
        {
            // 裁成2个新的三角形
            if (d0 < 0)
            {
                ShaderV2F vf1s = ShaderV2F::lerpV2F(vf0, vf1, d0 / (d0 - d1));
                ShaderV2F vf2s = ShaderV2F::lerpV2F(vf0, vf2, d0 / (d0 - d2));
                RasterizerScanline::drawTriangle(fbo, shader, vf1, vf2, vf1s);
                RasterizerScanline::drawTriangle(fbo, shader, vf1, vf2, vf2s);
                RasterizerWireframe::drawTriangle(fbo, shader, vf1, vf2, vf1s);
                RasterizerWireframe::drawTriangle(fbo, shader, vf1, vf2, vf2s);
            }
            else if (d1 < 0)
            {
                ShaderV2F vf0s = ShaderV2F::lerpV2F(vf1, vf0, d1 / (d1 - d0));
                ShaderV2F vf2s = ShaderV2F::lerpV2F(vf1, vf2, d1 / (d1 - d2));
                RasterizerScanline::drawTriangle(fbo, shader, vf0, vf2, vf0s);
                RasterizerScanline::drawTriangle(fbo, shader, vf0, vf2, vf2s);
                RasterizerWireframe::drawTriangle(fbo, shader, vf0, vf2, vf0s);
                RasterizerWireframe::drawTriangle(fbo, shader, vf0, vf2, vf2s);
            }
            else if (d2 < 0)
            {
                ShaderV2F vf0s = ShaderV2F::lerpV2F(vf2, vf0, d2 / (d2 - d0));
                ShaderV2F vf1s = ShaderV2F::lerpV2F(vf2, vf1, d2 / (d2 - d1));
                RasterizerScanline::drawTriangle(fbo, shader, vf0, vf1, vf0s);
                RasterizerScanline::drawTriangle(fbo, shader, vf0, vf1, vf1s);
                RasterizerWireframe::drawTriangle(fbo, shader, vf0, vf1, vf0s);
                RasterizerWireframe::drawTriangle(fbo, shader, vf0, vf1, vf1s);
            }
        }
        else if (insideCount == 3)
        {
            // 无需裁剪
            RasterizerScanline::drawTriangle(fbo, shader, vf0, vf1, vf2);
            RasterizerWireframe::drawTriangle(fbo, shader, vf0, vf1, vf2);
        }
    }
}
