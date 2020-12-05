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

    class RasterPipeline
    {
    public:
        static void drawTriangles(FrameBuffer& fbo, std::unique_ptr<Shader>& shader, const Mesh& mesh);
        static void drawTriangle(FrameBuffer& fbo, std::unique_ptr<Shader>& shader, const Vertex& v0, const Vertex& v1, const Vertex& v2);

        /*static void DrawTriangles(FrameBuffer& fbo, std::unique_ptr<Shader>& shader, const Mesh& mesh);

        static void VertTransform(std::unique_ptr<Shader>& shader, const Vertex& v0, const Vertex& v1, const Vertex& v2);
        static void CullingAndClipping();
        static void Rasterization();

        static bool DepthTest(float src, float dst);
        static bool StencilTest();
        static Vec4 AlphaBlend(const Vec4& src, const Vec4& dst);*/
    };
}
