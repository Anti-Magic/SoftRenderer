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

        FilterMode filterMode = FilterMode::Point;

        virtual Vec4 GetColor(const Vec4& pos) = 0;

        inline Vec4 GetColor(const Vec3& pos)
        {
            return GetColor(Vec4(pos, 0));
        }

        inline Vec4 GetColor(const Vec2& pos)
        {
            return GetColor(Vec4(pos, 0, 0));
        }
    };
}
