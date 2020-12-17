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
        Vec3 dir;
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

        Mat4 uniformM0;
        Vec4 uniformV0;
        Vec4 uniformV1;

        virtual ShaderV2F Vert(const Vertex& v) = 0;
        virtual Vec4 Frag(const ShaderV2F& f) = 0;
    };
}
