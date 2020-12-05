#include "FrameBuffer.h"

namespace SoftRenderer
{
    FrameBuffer::FrameBuffer(const Vec2& _size)
        : size(_size)
        , color(std::make_shared<Texture2D>(_size, Vec4(0, 0, 0, 1)))
        , depth(_size.x * _size.y, 1)
        , stencil(_size.x * _size.y, 0)
    {
    }

    void FrameBuffer::clear(const Vec4& _color)
    {
        for (size_t i = 0; i < depth.size(); i++)
        {
            color->d[i * 4] = _color.x;
            color->d[i * 4 + 1] = _color.y;
            color->d[i * 4 + 2] = _color.z;
            color->d[i * 4 + 3] = _color.w;
            depth[i] = 1;
            stencil[i] = 0;
        }
    }

    void FrameBuffer::setColor(int x, int y, const Vec4& value)
    {
        if (x < 0 || x >= size.x || y < 0 || y >= size.y)
        {
            return;
        }

        int index = (x + y * size.x) * 4;
        color->d[index] = value.x;
        color->d[index + 1] = value.y;
        color->d[index + 2] = value.z;
        color->d[index + 3] = value.w;
    }

    Vec4 FrameBuffer::getColor(int x, int y)
    {
        int index = (x + y * size.x) * 4;
        return Vec4(color->d[index], color->d[index + 1], color->d[index + 2], color->d[index + 3]);
    }

    void FrameBuffer::setDepth(int x, int y, float value)
    {
        if (x < 0 || x >= size.x || y < 0 || y >= size.y)
        {
            return;
        }

        int index = x + y * size.x;
        depth[index] = value;
    }

    float FrameBuffer::getDepth(int x, int y)
    {
        int index = x + y * size.x;
        return depth[index];
    }

    void FrameBuffer::setStencil(int x, int y, float value)
    {
        if (x < 0 || x >= size.x || y < 0 || y >= size.y)
        {
            return;
        }

        int index = x + y * size.x;
        stencil[index] = value;
    }

    float FrameBuffer::getStencil(int x, int y)
    {
        int index = x + y * size.x;
        return stencil[index];
    }
}
