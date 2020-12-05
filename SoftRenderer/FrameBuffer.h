#pragma once

#include "Mathf.h"
#include "Texture2D.h"
#include <memory>

namespace SoftRenderer
{
    class FrameBuffer
    {
    public:
        Vec2 size;
        std::shared_ptr<Texture2D> color;
        std::vector<float> depth;
        std::vector<float> stencil;

        FrameBuffer(const Vec2& _size);

        void clear(const Vec4& _color);

        void setColor(int x, int y, const Vec4& value);
        Vec4 getColor(int x, int y);

        void setDepth(int x, int y, float value);
        float getDepth(int x, int y);

        void setStencil(int x, int y, float value);
        float getStencil(int x, int y);
    };
}
