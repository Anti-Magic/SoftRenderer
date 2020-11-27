#pragma once

#include "Shader.h"

namespace SoftRenderer
{
    class ShaderColor : public Shader
    {
    public:
        void vert(const Vertex& v, ShaderV2F& o) override
        {
            o.position = v.position * mvp;
            o.texcoord0 = v.texcoord;
        }

        Vec4 frag(const ShaderV2F& f) override
        {
            return Vec4(f.texcoord0.x, f.texcoord0.y, f.texcoord0.z, 1);
        }
    };
}
