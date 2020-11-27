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

    class RasterizationParam
    {
    public:
        FrameBuffer& fbo;
        std::unique_ptr<Shader>& shader;
        std::array<ShaderV2F, 3> v;
        ShaderV2F vfl;
        ShaderV2F vfr;
        ShaderV2F vfm;

        RasterizationParam(FrameBuffer& _fbo, std::unique_ptr<Shader>& _shader)
            : fbo(_fbo)
            , shader(_shader)
        {
        }
    };

    class Rasterization
    {
    public:
        static void drawLine(FrameBuffer& fbo, const Vec4& begin, const Vec4& end);
        static void drawTriangles(FrameBuffer& fbo, std::unique_ptr<Shader>& shader, const Mesh& mesh);
        static bool cvvCulling(const ShaderV2F& v);
        static void drawTriangle(RasterizationParam& param, const Vertex& v0, const Vertex& v1, const Vertex& v2);
        static void drawFlatTopTriangle(RasterizationParam& param);
        static void drawFlatBotTriangle(RasterizationParam& param);
        static void drawScanLine(RasterizationParam& param, int y);
        static bool depthTest(float src, float dst);
        static Vec4 alphaBlend(const Vec4& src, const Vec4& dst);
    };
}
