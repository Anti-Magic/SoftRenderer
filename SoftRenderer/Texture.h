#pragma once

#include "Mathf.h"

namespace SoftRenderer
{
    class Texture
    {
    public:
        enum class FilterMode
        {
            Point,
            Bilinear
        };

        FilterMode filterMode = FilterMode::Bilinear;

        virtual Vec4 getColor(const Vec4& pos) = 0;

        inline Vec4 getColor(const Vec3& pos)
        {
            return Vec4(pos, 0);
        }

        inline Vec4 getColor(const Vec2& pos)
        {
            return Vec4(pos, 0, 0);
        }
    };
}
