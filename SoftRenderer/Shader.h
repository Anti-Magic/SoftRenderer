#pragma once

#include "Mathf.h"
#include "ShaderV2F.h"
#include "Vertex.h"
#include "Texture.h"
#include <memory>

namespace SoftRenderer
{
    class Lighting
    {
    public:
        Vec4 ambient;
        Vec4 dir;
        Vec4 color;
    };

    class Shader
    {
    public:
        Mat4 mvp;
        Mat4 mModel;
        Mat4 mNormal;
        Vec4 posView;
        Vec2 resolution;
        float gloss;
        Lighting lighting;
        std::shared_ptr<Texture> texture0;
        std::shared_ptr<Texture> texture1;

        virtual void vert(const Vertex& v, ShaderV2F& o) = 0;
        virtual Vec4 frag(const ShaderV2F& f) = 0;
    };
}
