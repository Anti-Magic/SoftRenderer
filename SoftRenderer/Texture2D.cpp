#include "Texture2D.h"

namespace SoftRenderer
{
    Texture2D::Texture2D(const Vec2& _size, const Vec4& value)
        : Texture()
        , size(_size)
        , d(_size.x * _size.y * 4)
    {
        for (size_t i = 0; i < d.size(); i += 4)
        {
            d[i] = value.x;
            d[i + 1] = value.y;
            d[i + 2] = value.z;
            d[i + 3] = value.w;
        }
    }

    Vec4 Texture2D::GetColor(const Vec4& pos)
    {
        float x = pos.x * size.x;
        float y = pos.y * size.y;
        x = clamp(x, 0.0f, size.x - 1);
        y = clamp(y, 0.0f, size.y - 1);
        if (filterMode == FilterMode::Point)
        {
            return GetColorVec4(floor(x), floor(y));
        }
        else
        {
            Vec4 ld = GetColorVec4(floor(x), floor(y));
            Vec4 rd = GetColorVec4(ceil(x), floor(y));
            Vec4 lu = GetColorVec4(floor(x), ceil(y));
            Vec4 ru = GetColorVec4(ceil(x), ceil(y));
            Vec4 hd = lerp(ld, rd, x - floor(x));
            Vec4 hu = lerp(lu, ru, x - floor(x));
            return lerp(hd, hu, y - floor(y));
        }
    }

    Vec4 Texture2D::GetColorVec4(int x, int y)
    {
        int index = (x + y * size.x) * 4;
        return Vec4(d[index], d[index + 1], d[index + 2], d[index + 3]);
    }
}
