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

    class RasterizerScanline
    {
    public:
        static void drawTriangle(FrameBuffer& fbo, std::unique_ptr<Shader>& shader, ShaderV2F v0, ShaderV2F v1, ShaderV2F v2, const RasterState& rState);
        static void drawFlatTopTriangle(FrameBuffer& fbo, std::unique_ptr<Shader>& shader, const ShaderV2F& v0, const ShaderV2F& v1, const ShaderV2F& v2, const RasterState& rState);
        static void drawFlatBotTriangle(FrameBuffer& fbo, std::unique_ptr<Shader>& shader, const ShaderV2F& v0, const ShaderV2F& v1, const ShaderV2F& v2, const RasterState& rState);
        static void drawScanline(FrameBuffer& fbo, std::unique_ptr<Shader>& shader, const ShaderV2F& vl, const ShaderV2F& vr, int y, const RasterState& rState);
    };
}
