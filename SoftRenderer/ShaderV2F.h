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

        static ShaderV2F baryerp(const ShaderV2F& v0, const ShaderV2F& v1, const ShaderV2F& v2, const Vec3& weight)
        {
            ShaderV2F o;
            o.position = v0.position * weight.x + v1.position * weight.y + v2.position * weight.z;
            o.normal = v0.normal * weight.x + v1.normal * weight.y + v2.normal * weight.z;
            o.texcoord0 = v0.texcoord0 * weight.x + v1.texcoord0 * weight.y + v2.texcoord0 * weight.z;
            o.texcoord1 = v0.texcoord1 * weight.x + v1.texcoord1 * weight.y + v2.texcoord1 * weight.z;
            o.texcoord2 = v0.texcoord2 * weight.x + v1.texcoord2 * weight.y + v2.texcoord2 * weight.z;
            o.texcoord3 = v0.texcoord3 * weight.x + v1.texcoord3 * weight.y + v2.texcoord3 * weight.z;
            return o;
        }
    };
}
