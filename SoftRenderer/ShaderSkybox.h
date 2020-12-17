#pragma once

#include "Shader.h"

namespace SoftRenderer
{
    class ShaderSkybox : public Shader
    {
    public:
        ShaderV2F Vert(const Vertex& v) override
        {
            ShaderV2F o;
            o.position = v.position * mvp;
            //o.position.z = o.position.w;
            o.texcoord0 = v.position;
            return o;
        }

        Vec4 Frag(const ShaderV2F& f) override
        {
            return texture0->GetColor(f.texcoord0);
        }
    };
}
