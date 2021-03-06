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

    class RasterizerWireframe
    {
    public:
        static void DrawTriangle(FrameBuffer& fbo, std::unique_ptr<Shader>& shader, ShaderV2F v0, ShaderV2F v1, ShaderV2F v2, const RasterState& rState);

	private:
		static void DrawLineLerp(FrameBuffer& fbo, const Vec4& begin, const Vec4& end, const Vec4& color);
		static void DrawLineBresenham(FrameBuffer& fbo, const Vec4& begin, const Vec4& end, const Vec4& color);
    };
}
