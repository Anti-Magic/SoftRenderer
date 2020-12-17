#pragma once

#include "Shader.h"

namespace SoftRenderer
{
    class ShaderUnlit : public Shader
    {
    public:
        ShaderV2F Vert(const Vertex& v) override
        {
            ShaderV2F o;
            o.position = v.position * mvp;
            o.texcoord0 = v.texcoord;
            return o;
        }

        Vec4 Frag(const ShaderV2F& f) override
        {
            return texture0->GetColor(f.texcoord0);
        }
    };
}
