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
    void RasterPipeline::DrawTriangles(FrameBuffer& fbo, std::unique_ptr<Shader>& shader, const Mesh& mesh, const RasterState& rState)
    {
        if (mesh.indices.size() > 0)
        {
            for (size_t i = 0; i < mesh.indices.size(); i += 3)
            {
				DrawTriangle(fbo, shader, mesh.vertices[mesh.indices[i]], mesh.vertices[mesh.indices[i + 1]], mesh.vertices[mesh.indices[i + 2]], rState);
            }
        }
        else
        {
            for (size_t i = 0; i < mesh.vertices.size(); i += 3)
            {
				DrawTriangle(fbo, shader, mesh.vertices[i], mesh.vertices[i + 1], mesh.vertices[i + 2], rState);
            }
        }
    }

    void RasterPipeline::DrawTriangle(FrameBuffer& fbo, std::unique_ptr<Shader>& shader, const Vertex& v0Raw, const Vertex& v1Raw, const Vertex& v2Raw, const RasterState& rState)
    {
		std::array<ShaderV2F, 3> v 
		{ 
			shader->Vert(v0Raw), 
			shader->Vert(v1Raw), 
			shader->Vert(v2Raw), 
		};

        std::vector<ShaderV2F> clippedVerts = Clipping(v, rState);

        for (auto& v : clippedVerts)
        {
            PerspectiveDivision(v);
            PerspectiveCorrect(v);
            ViewPortTransform(v, fbo.size);
        }

		for (int i = 2; i < clippedVerts.size(); i++)
		{
			auto& cv0 = clippedVerts[0];
			auto& cv1 = clippedVerts[i - 1];
			auto& cv2 = clippedVerts[i];
			if (Culling(cv0, cv1, cv2, rState))
			{
				continue;
			}
			if ((rState.rasterMode & RasterMode::Fill) != RasterMode::None)
			{
				if (rState.rasterMethod == RasterMethod::HalfSpace)
				{
					RasterizerHalfSpace::DrawTriangle(fbo, shader, cv0, cv1, cv2, rState);
				}
				else
				{
					RasterizerScanline::DrawTriangle(fbo, shader, cv0, cv1, cv2, rState);
				}
			}
			if ((rState.rasterMode & RasterMode::Wireframe) != RasterMode::None)
			{
				RasterizerWireframe::DrawTriangle(fbo, shader, cv0, cv1, cv2, rState);
			}
		}
    }

    void RasterPipeline::PerspectiveDivision(ShaderV2F& v)
    {
        v.position.x /= v.position.w;
        v.position.y /= v.position.w;
        v.position.z /= v.position.w;
    }

	// Sutherland–Hodgman算法，方便切成三角扇以及保证环绕顺序
	// 若三角形边的两个端点都在内侧，则将第二个顶点加入到输出列表
	// 若三角形边的第一个端点在内侧而第二个端点在外侧，则将交点加入到输出列表
	// 若三角形边的第一个端点在外侧而第二个端点在内侧，则将交点以及第二个顶点依次加入到输出列表
	// 若三角形边的两个端点都在外侧，则什么都不做。
    std::vector<ShaderV2F> RasterPipeline::Clipping(const std::array<ShaderV2F, 3>& v, const RasterState& rState)
    {
        if (!rState.enableClipping)
        {
            return std::vector<ShaderV2F> {v[0], v[1], v[2]};
        }

		std::vector<ShaderV2F> result;

        // 只对近裁面做裁剪
        ClippingPlane near(Vec3(0, 0, 1), 0);
		std::array<float, 3> d { 
			near.DistanceFromPoint4(v[0].position), 
			near.DistanceFromPoint4(v[1].position), 
			near.DistanceFromPoint4(v[2].position)
		};
		for (int i = 0; i < 3; i++)
		{
			int j = (i + 1) % 3;
			if (d[i] >= 0 && d[j] >= 0)
			{
				result.push_back(v[j]);
			}
			else if (d[i] >= 0 && d[j] < 0)
			{
				result.push_back(ShaderV2F::Lerp(v[i], v[j], d[i] / (d[i] - d[j])));
			}
			else if (d[i] < 0 && d[j] >= 0)
			{
				result.push_back(ShaderV2F::Lerp(v[i], v[j], d[i] / (d[i] - d[j])));
				result.push_back(v[j]);
			}
		}
		return result;
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
        v.PerspectiveCorrect(v.position.w);
    }

    void RasterPipeline::ViewPortTransform(ShaderV2F& v0, const Vec2& size)
    {
        v0.position.x = (v0.position.x + 1) * 0.5f * size.x;
        v0.position.y = (v0.position.y + 1) * 0.5f * size.y;
    }
}
