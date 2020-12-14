#pragma once

#include "Texture2D.h"
#include <string>
#include <memory>

namespace SoftRenderer
{
    class TextureCube : public Texture
    {
    public:
        TextureCube(
            std::shared_ptr<Texture2D> _left,
            std::shared_ptr<Texture2D> _right,
            std::shared_ptr<Texture2D> _bottom,
            std::shared_ptr<Texture2D> _top,
            std::shared_ptr<Texture2D> _back,
            std::shared_ptr<Texture2D> _front);

        Vec4 getColor(const Vec4& pos) override;

    private:
        std::shared_ptr<Texture2D> left;
        std::shared_ptr<Texture2D> right;
        std::shared_ptr<Texture2D> bottom;
        std::shared_ptr<Texture2D> top;
        std::shared_ptr<Texture2D> back;
        std::shared_ptr<Texture2D> front;
    };
}
