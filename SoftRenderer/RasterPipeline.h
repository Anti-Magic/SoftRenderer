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
    class Mesh;
    class Vertex;
    class RasterState;

    class RasterPipeline
    {
    public:
        static void drawTriangles(FrameBuffer& fbo, std::unique_ptr<Shader>& shader, const Mesh& mesh, const RasterState& rState);
        static void drawTriangle(FrameBuffer& fbo, std::unique_ptr<Shader>& shader, const Vertex& v0Raw, const Vertex& v1Raw, const Vertex& v2Raw, const RasterState& rState);
        
	private:
        static void PerspectiveDivision(ShaderV2F& v);
        static void PerspectiveCorrect(ShaderV2F& v);
        static void ViewPortTransform(ShaderV2F& v, const Vec2& size);
        static std::vector<ShaderV2F> Clipping(const ShaderV2F& v0, const ShaderV2F& v1, const ShaderV2F& v2, const RasterState& rState);
        static bool Culling(const ShaderV2F& v0, const ShaderV2F& v1, const ShaderV2F& v2, const RasterState& rState);
    };
}
