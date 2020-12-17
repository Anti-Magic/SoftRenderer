#pragma once

#include "Mathf.h"
#include <array>
#include <memory>
#include <vector>
#include "ShaderV2F.h"

namespace SoftRenderer
{
    class FrameBuffer;
    class Shader;
    class RasterState;

    class RasterizerHalfSpace
    {
    public:
        static void drawTriangle(FrameBuffer& fbo, std::unique_ptr<Shader>& shader, ShaderV2F v0, ShaderV2F v1, ShaderV2F v2, const RasterState& rState);
    
    private:
        static Vec3 calcWeight(const ShaderV2F& v0, const ShaderV2F& v1, const ShaderV2F& v2, const Vec2& p);
    };
}
