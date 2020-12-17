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

        void Clear(const Vec4& _color);

        void SetColor(int x, int y, const Vec4& value);
        Vec4 GetColor(int x, int y);

        void SetDepth(int x, int y, float value);
        float GetDepth(int x, int y);

        void SetStencil(int x, int y, float value);
        float GetStencil(int x, int y);
    };
}
