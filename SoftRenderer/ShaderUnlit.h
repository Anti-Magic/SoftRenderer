#pragma once

#include "Shader.h"

namespace SoftRenderer
{
    class ShaderUnlit : public Shader
    {
    public:
        void vert(const Vertex& v, ShaderV2F& o) override
        {
            o.position = v.position * mvp;
            o.texcoord0 = v.texcoord;
        }

        Vec4 frag(const ShaderV2F& f) override
        {
            return texture0->getColor(f.texcoord0);
        }
    };
}
