#pragma once

#include "Texture.h"
#include <vector>
#include "Mathf.h"

namespace SoftRenderer
{
    class Texture2D : public Texture
    {
    public:
        Vec2 size;
        std::vector<float> d;

        Texture2D(const Vec2& _size, const Vec4& value);
        Vec4 getColor(const Vec4& pos) override;

    private:
        Vec4 getColorVec4(int x, int y);
    };
}
