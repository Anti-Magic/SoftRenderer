#include "RasterizerHalfSpace.h"
#include "RasterState.h"
#include "FrameBuffer.h"
#include "Shader.h"
#include "Mesh.h"

namespace SoftRenderer
{
    void RasterizerHalfSpace::drawTriangle(FrameBuffer& fbo, std::unique_ptr<Shader>& shader, ShaderV2F v0, ShaderV2F v1, ShaderV2F v2, const RasterState& rState)
    {
        int minX = clamp(std::min(std::min(v0.position.x, v1.position.x), v2.position.x), 0.0f, fbo.size.x - 1);
        int maxX = clamp(std::max(std::max(v0.position.x, v1.position.x), v2.position.x), 0.0f, fbo.size.x - 1);
        int minY = clamp(std::min(std::min(v0.position.y, v1.position.y), v2.position.y), 0.0f, fbo.size.y - 1);
        int maxY = clamp(std::max(std::max(v0.position.y, v1.position.y), v2.position.y), 0.0f, fbo.size.y - 1);
        for (int y = minY; y <= maxY; y++)
        {
            for (int x = minX; x <= maxX; x++)
            {
                Vec3 weight = calcWeight(v0, v1, v2, Vec2(x, y));
                if (weight.x >= 0 && weight.y >= 0 && weight.z >= 0)
                {
                    ShaderV2F vm = ShaderV2F::baryerp(v0, v1, v2, weight);
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
    }

    Vec3 RasterizerHalfSpace::calcWeight(const ShaderV2F& v0, const ShaderV2F& v1, const ShaderV2F& v2, const Vec2& p)
    {
        Vec2 v01(v1.position.x - v0.position.x, v1.position.y - v0.position.y);
        Vec2 v02(v2.position.x - v0.position.x, v2.position.y - v0.position.y);
        Vec2 v0p(p.x - v0.position.x, p.y - v0.position.y);
        float areaTotal = v01.cross(v02);
		float weight1 = v0p.cross(v02) / areaTotal;
        float weight2 = v01.cross(v0p) / areaTotal;
        return Vec3(1 - weight1 - weight2, weight1, weight2);
    }
}
