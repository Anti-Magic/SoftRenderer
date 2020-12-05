#pragma once

#include "Shader.h"

namespace SoftRenderer
{
    class ShaderDepth : public Shader
    {
    public:
        ShaderV2F vert(const Vertex& v) override
        {
            ShaderV2F o;
            o.position = v.position * mvp;
            o.texcoord3 = v.position * mvp;
            return o;
        }

        Vec4 frag(const ShaderV2F& f) override
        {
            return Vec4(f.position.z, 0, 0, 1);
        }
    };
}
