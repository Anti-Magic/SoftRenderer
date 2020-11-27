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

        // TODO
        void mul(float v)
        {
            normal = normal * v;
            texcoord0 = texcoord0 * v;
            texcoord1 = texcoord1 * v;
            texcoord2 = texcoord2 * v;
            texcoord3 = texcoord3 * v;
        }

        // TODO
        void fromLerp(const ShaderV2F& a, const ShaderV2F& b, float t)
        {
            position = lerp(a.position, b.position, t);
            normal = lerp(a.normal, b.normal, t);
            texcoord0 = lerp(a.texcoord0, b.texcoord0, t);
            texcoord1 = lerp(a.texcoord1, b.texcoord1, t);
            texcoord2 = lerp(a.texcoord2, b.texcoord2, t);
            texcoord3 = lerp(a.texcoord3, b.texcoord3, t);
        }
    };
}
