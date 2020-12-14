#pragma once

#include "Mathf.h"

namespace SoftRenderer
{
    class ShaderV2F
    {
    public:
        Vec4 position;
        Vec4 normal;
        Vec4 texcoord0;
        Vec4 texcoord1;
        Vec4 texcoord2;
        Vec4 texcoord3;

        void perspectiveCorrect(float v)
        {
            normal = normal * v;
            texcoord0 = texcoord0 * v;
            texcoord1 = texcoord1 * v;
            texcoord2 = texcoord2 * v;
            texcoord3 = texcoord3 * v;
        }

        static ShaderV2F lerp(const ShaderV2F& a, const ShaderV2F& b, float t)
        {
            ShaderV2F o;
            o.position = SoftRenderer::lerp(a.position, b.position, t);
            o.normal = SoftRenderer::lerp(a.normal, b.normal, t);
            o.texcoord0 = SoftRenderer::lerp(a.texcoord0, b.texcoord0, t);
            o.texcoord1 = SoftRenderer::lerp(a.texcoord1, b.texcoord1, t);
            o.texcoord2 = SoftRenderer::lerp(a.texcoord2, b.texcoord2, t);
            o.texcoord3 = SoftRenderer::lerp(a.texcoord3, b.texcoord3, t);
            return o;
        }
    };
}
