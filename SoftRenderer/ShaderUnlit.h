#pragma once

#include "Shader.h"

namespace SoftRenderer
{
    class ShaderUnlit : public Shader
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
            return texture0->getColor(f.texcoord0);
        }
    };
}
