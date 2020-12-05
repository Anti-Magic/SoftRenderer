#pragma once

#include "Shader.h"

namespace SoftRenderer
{
    class ShaderColor : public Shader
    {
    public:
        ShaderV2F vert(const Vertex& v) override
        {
            ShaderV2F o;
            o.position = v.position * mvp;
            o.texcoord0 = v.texcoord;
            return o;
        }

        Vec4 frag(const ShaderV2F& f) override
        {
            return Vec4(f.texcoord0.x, f.texcoord0.y, f.texcoord0.z, 1);
        }
    };
}
