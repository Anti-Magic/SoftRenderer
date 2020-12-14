#include "TextureCube.h"
#include "Texture.h"
#include "Device.h"

namespace SoftRenderer
{
    TextureCube::TextureCube(
        std::shared_ptr<Texture2D> _left,
        std::shared_ptr<Texture2D> _right,
        std::shared_ptr<Texture2D> _bottom,
        std::shared_ptr<Texture2D> _top,
        std::shared_ptr<Texture2D> _back,
        std::shared_ptr<Texture2D> _front)
        : Texture()
        , left(_left)
        , right(_right)
        , bottom(_bottom)
        , top(_top)
        , back(_back)
        , front(_front)
    {
    }

    Vec4 TextureCube::getColor(const Vec4& pos)
    {
        float absx = abs(pos.x);
        float absy = abs(pos.y);
        float absz = abs(pos.z);
        std::shared_ptr<Texture2D> dstTex;
        float sc;
        float tc;
        float ma;
        if (absx >= absy && absx >= absz)
        {
            ma = absx;
            if (pos.x > 0)
            {
                dstTex = right;
                sc = -pos.z;
                tc = -pos.y;
            }
            else
            {
                dstTex = left;
                sc = pos.z;
                tc = -pos.y;
            }
        }
        else if (absy >= absx && absy >= absz)
        {
            ma = absy;
            if (pos.y > 0)
            {
                dstTex = top;
                sc = pos.x;
                tc = pos.z;
            }
            else
            {
                dstTex = bottom;
                sc = pos.x;
                tc = -pos.z;
            }
        }
        else //if (absz >= absx && absz >= absy)
        {
            ma = absz;
            if (pos.z > 0)
            {
                dstTex = back;
                sc = pos.x;
                tc = -pos.y;
            }
            else
            {
                dstTex = front;
                sc = -pos.x;
                tc = -pos.y;
            }
        }

        float s = 0.5f * (sc / ma + 1.0f);
        float t = 0.5f * (tc / ma + 1.0f);
        return dstTex->getColor(Vec2(s, t));
    }
}
